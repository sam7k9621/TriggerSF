import ROOT
import subprocess
import operator
import CPVAnalysis.CompareDataMC.PlotMgr as pltmgr
import CPVAnalysis.CompareDataMC.ParseMgr as parmgr
import TriggerSF.SFAnlzr.PlotInfo as mysetting

def SetHist( hist, width=2, color=1, marker=1, size=1 ):
    hist.SetLineWidth( width )
    hist.SetLineColor( color )

    hist.SetMarkerColor( color )
    hist.SetMarkerSize( size )
    hist.SetMarkerStyle( marker )


def MakeSampleName( sample ):
    lst = sample.split("_")
    return "{}-{}_{}".format( lst[1], lst[2], lst[-1])

def CheckFist( obj ):
    datalst = getattr( mysetting, "datalst_{}".format( opt.Year() ) )
    hlt      = getattr( mysetting, "HLT_{}".format( opt.Year() ) )   
    
    c = pltmgr.NewCanvas()
    histlst = []
    leg = pltmgr.NewLegend( 0.2, 0.2, 0.4, 0.6 )
    leg.SetHeader( obj.split("_")[0].capitalize(), "C" ) 
    for i, era in enumerate( datalst ):
        histlst.append( histmgr.GetMergedObj( era[0], operator.contains ) )
        histlst[i].SetLineColor( pltmgr.colorlst[i] )
        histlst[i].SetLineWidth( 2 )
        leg.AddEntry( histlst[i], era[0], "L" )
    
    
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
    pltmgr.DrawEntryRight( hlt )
    
    c.SaveAs( "results/Eff{}_{}.pdf".format( opt.Year(), obj ) )

def PlotEtaEff( obj ):
    datalst  = getattr( mysetting, "datalst_{}".format( opt.Year() ) )
    mcsample = getattr( mysetting, "mc_{}"  .format( opt.Year() ) )
    hlt      = getattr( mysetting, "HLT_{}".format( opt.Year() ) )   
    
    mc = histmgr.GetMergedObj( mcsample,   operator.contains )
    mc = mc.CreateGraph()
    for era in datalst: 
        leg  = pltmgr.NewLegend( 0.2, 0.4, 0.4, 0.8 )
        data = histmgr.GetMergedObj( era[0], operator.contains )
        data = data.CreateGraph()

        c = pltmgr.NewCanvas( era[0] )
        top = pltmgr.NewTopPad()
        top.Draw()
        top.cd()

        frame1 = ROOT.gPad.DrawFrame( -3, 0, 3, 1.4 )
        data.Draw( "EP" )
        mc  .Draw("EP" )
        
        SetHist( data, color=ROOT.kGray+1,  marker=33 )
        SetHist( mc,   color=ROOT.kAzure-3, marker=8 )

        leg = pltmgr.NewLegend( 0.55, 0.18, 0.65, 0.35 )
        leg.AddEntry( data, era[0], "lp" )
        leg.AddEntry( mc,   "MC",   "lp" )
        leg.Draw()

        line1 = ROOT.TLine( -3, 1, 3, 1 )
        line1.SetLineColor( ROOT.kRed )
        line1.SetLineStyle( 8 )
        line1.Draw()

        frame1.GetXaxis().SetTitle( "" )
        frame1.GetYaxis().SetTitle( "Efficiency" )
        pltmgr.SetTopPlotAxis( frame1 )  
   
        c.cd()
        
        bot = pltmgr.NewBottomPad()
        bot.Draw() 
        bot.cd() 
        
        frame2 = ROOT.gPad.DrawFrame( -3, 0.85, 3, 1.15 )
        frame2.GetXaxis().SetTitle( "\eta" )
        frame2.GetYaxis().SetTitle( "Data/MC" )
  
        pltmgr.SetBottomPlotAxis( frame2 )
        scale = pltmgr.DivideGraph( data, mc )
        scale.Draw( "EP" )
        scale.SetLineColor( 1 )
        scale.SetMarkerColor( 1 )
        scale.SetMarkerStyle( 20 )
        
        line2 = ROOT.TLine( -3, 1, 3, 1 )
        line2.SetLineColor( ROOT.kRed )
        line2.SetLineStyle( 8 )
        line2.Draw()

        c.cd()

        pltmgr.DrawCMSLabel( pltmgr.PRELIMINARY )
        pltmgr.DrawLuminosity( era[1] )
        pltmgr.DrawEntryLeft( hlt )
        
        c.SaveAs( "results/Eff{}_{}_{}_{}.pdf".format( opt.Year(), era[0], mcsample, obj ) )
        pltmgr.SaveToROOT( scale, "results/Eff{}_{}_{}.root".format( opt.Year(), era[0], mcsample ), obj )
        
def PlotPtEff( obj ):
    datalst  = getattr( mysetting, "datalst_{}".format( opt.Year() ) )
    mcsample = getattr( mysetting, "mc_{}"  .format( opt.Year() ) )
    hlt      = getattr( mysetting, "HLT_{}".format( opt.Year() ) )   
    
    mc = histmgr.GetMergedObj( mcsample,   operator.contains )
    mc = mc.CreateGraph()
    for era in datalst: 
        leg  = pltmgr.NewLegend( 0.2, 0.4, 0.4, 0.8 )
        data = histmgr.GetMergedObj( era[0], operator.contains )
        data = data.CreateGraph()

        c = pltmgr.NewCanvas( era[0] )
        top = pltmgr.NewTopPad()
        top.Draw()
        top.cd()

        frame1 = ROOT.gPad.DrawFrame( 0, 0, 600, 1.4 )
        data.Draw( "EP" )
        mc  .Draw("EP" )
        
        SetHist( data, color=ROOT.kGray+1,  marker=33 )
        SetHist( mc,   color=ROOT.kAzure-3, marker=8 )

        leg = pltmgr.NewLegend( 0.65, 0.23, 0.75, 0.4 )
        leg.AddEntry( data, era[0], "lp" )
        leg.AddEntry( mc,   "MC",   "lp" )
        leg.Draw()

        line1 = ROOT.TLine( 0, 1, 600, 1 )
        line1.SetLineColor( ROOT.kRed )
        line1.SetLineStyle( 8 )
        line1.Draw()

        frame1.GetXaxis().SetTitle( "" )
        frame1.GetYaxis().SetTitle( "Efficiency" )
        pltmgr.SetTopPlotAxis( frame1 )  
   
        c.cd()
        
        bot = pltmgr.NewBottomPad()
        bot.Draw() 
        bot.cd() 
        
        frame2 = ROOT.gPad.DrawFrame( 0, 0.85, 600, 1.15 )
        frame2.GetXaxis().SetTitle( "P_{T}" )
        frame2.GetYaxis().SetTitle( "Data/MC" )
  
        pltmgr.SetBottomPlotAxis( frame2 )
        scale = pltmgr.DivideGraph( data, mc )
        scale.Draw( "EP" )
        scale.SetLineColor( 1 )
        scale.SetMarkerColor( 1 )
        scale.SetMarkerStyle( 20 )
        
        line2 = ROOT.TLine( 0, 1, 600, 1 )
        line2.SetLineColor( ROOT.kRed )
        line2.SetLineStyle( 8 )
        line2.Draw()

        c.cd()

        pltmgr.DrawCMSLabel( pltmgr.PRELIMINARY )
        pltmgr.DrawLuminosity( era[1] )
        pltmgr.DrawEntryLeft( hlt )
        
        c.SaveAs( "results/Eff{}_{}_{}_{}.pdf".format( opt.Year(), era[0], mcsample, obj ) )
        pltmgr.SaveToROOT( scale, "results/Eff{}_{}_{}.root".format( opt.Year(), era[0], mcsample ), obj )

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

    PlotPtEff ( "pt" )
    PlotEtaEff( "eta" )
    PlotPtEff ( "pt_shift" )
    PlotEtaEff( "eta_shift" )
    PlotPtEff ( "pt_zmass" )
    PlotEtaEff( "eta_zmass" )
    CheckFist( "pass_zmass" )
    CheckFist( "fail_zmass" )

if __name__ == '__main__':
    main()
