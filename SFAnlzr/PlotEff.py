import ROOT
import subprocess
import operator
import CPVAnalysis.CompareDataMC.PlotMgr as pltmgr
import CPVAnalysis.CompareDataMC.ParseMgr as parmgr
import TriggerSF.SFAnlzr.PlotInfo as mysetting

def MakeSampleName( sample ):
    lst = sample.split("_")
    # print "{}-{}_{}".format( lst[1], lst[2], lst[-1])
    return "{}-{}_{}".format( lst[1], lst[2], lst[-1])

def CheckFist( obj ):
    datalst = getattr( mysetting, "datalst_{}".format( opt.Year() ) )
    leg = pltmgr.NewLegend( 0.2, 0.4, 0.4, 0.8 )
    leg.SetHeader( obj.split("_")[0].capitalize(), "C" ) 
    histlst = []
    for i, era in enumerate( datalst ):
        histlst.append( histmgr.GetMergedObj( era, operator.contains ) )
        histlst[i].SetLineColor( pltmgr.colorlst[i] )
        histlst[i].SetLineWidth( 1 )
        leg.AddEntry( histlst[i], era.split("-")[1], "L" )
    
    c = pltmgr.NewCanvas()
    
    histlst[0].SetTitle("")
    histlst[0].GetYaxis().SetTitle( "Events" )
    histlst[0].GetXaxis().SetTitle( "GeV" )
    histlst[0].SetStats( 0 )
    histlst[0].SetMaximum( max( [ pltmgr.GetHistYmax( x ) for x in histlst ] ) * 1.3 );
    histlst[ 0 ].Draw()
    for h in histlst:
        h.Draw( "same" )
    leg.Draw()
    
    pltmgr.SetSinglePad( c )
    pltmgr.SetAxis( histlst[ 0 ] )
    pltmgr.DrawCMSLabelOuter( pltmgr.PRELIMINARY )
    pltmgr.DrawEntryRight( "aaaa" )
    
    c.SaveAs( "results/Hist{}_{}.pdf".format( opt.Year(), obj ) )

def PlotPtEff( obj ):
    datalst = getattr( mysetting, "datalst_{}".format( opt.Year() ) )
    mclst   = getattr( mysetting, "mclst_{}"  .format( opt.Year() ) )

    leg  = pltmgr.NewLegend( 0.2, 0.4, 0.4, 0.8 )
    data = histmgr.GetMergedObj( datalst[0], operator.contains )
    mc   = histmgr.GetMergedObj( mclst[0],   operator.contains )

    data = data.CreateGraph()
    mc   = mc.  CreateGraph()

    c = pltmgr.NewCanvas()
    top = pltmgr.NewTopPad()
    top.Draw()
    top.cd()

    h1 = ROOT.gPad.DrawFrame( 0, 0, 500, 1.2 )
    data.Draw( "EP" )
    mc  .Draw("EP" )
    c.SaveAs( "results/Hist{}_{}.pdf".format( opt.Year(), obj ) )
    
# Initialize parsing manager
opt = parmgr.Parsemgr()
opt.Parsing()

# Initialize plot manager
histmgr = pltmgr.Plotmgr()
objlst  = getattr( mysetting, "objlst_{}".format( opt.Year() ) )

def main() :
    s = subprocess.Popen( 'ls results/TnP{}*'.format( opt.Year() ), shell=True, stdout=subprocess.PIPE )
    samplelst, err = s.communicate()
    samplelst = filter( lambda x: ".root" in x, samplelst.split('\n') )  
    for sample in samplelst:
        histmgr.SetObjlst( sample, objlst, MakeSampleName( sample ), "makeEff" )

    PlotPtEff( "pt_eff" )
    # CheckFist( "pass_zmass_Ele32" )
    # CheckFist( "fail_zmass_Ele32" )

if __name__ == '__main__':
    main()
