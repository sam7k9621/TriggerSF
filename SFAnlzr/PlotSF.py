
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

def PlotEtaUnc( nom, unc, header):
    hlt      = getattr( mysetting, "HLT_{}".format( opt.Year() ) )   
    c = pltmgr.NewCanvas()
    top = pltmgr.NewTopPad()
    top.Draw()
    top.cd()

    frame1 = ROOT.gPad.DrawFrame( -3, 0.6, 3, 1.6 )
    nom.Draw( "EP" )
    unc.Draw( "EP" )

    SetHist( nom, color=ROOT.kGray+1,  marker=33 )
    SetHist( unc, color=ROOT.kAzure-3, marker=8 )

    leg = pltmgr.NewLegend( 0.6, 0.6, 0.8, 0.8 )
    leg.SetHeader( header )
    leg.AddEntry( nom, "Nominal", "lp" )
    leg.AddEntry( unc, "Uncertainty", "lp" )
    leg.Draw()

    frame1.GetXaxis().SetTitle( "" )
    frame1.GetYaxis().SetTitle( "Scale factor" )
    pltmgr.SetTopPlotAxis( frame1 )

    c.cd()

    bot = pltmgr.NewBottomPad()
    bot.Draw()
    bot.cd()

    frame2 = ROOT.gPad.DrawFrame( -3, -0.055, 3, 0.055 )
    frame2.GetXaxis().SetTitle( "\eta" )
    frame2.GetYaxis().SetTitle( "Syst Unc" )

    pltmgr.SetBottomPlotAxis( frame2 )
    scale = pltmgr.DivideGraph( unc, nom, 1 )
    scale.Draw( "EP" )
    scale.SetLineColor( 1 )
    scale.SetMarkerColor( 1 )
    scale.SetMarkerStyle( 20 )

    line = ROOT.TLine( -3, 0, 3, 0 )
    line.SetLineColor( ROOT.kRed )
    line.SetLineStyle( 8 )
    line.Draw()

    c.cd()

    pltmgr.DrawCMSLabel( pltmgr.PRELIMINARY )
    pltmgr.DrawLuminosity( opt.Lumi() )
    pltmgr.DrawEntryLeft( hlt ) 

    c.SaveAs( "results/SF{}_eta_{}.pdf".format( opt.Year(), unc.GetName() ) )

def PlotPtUnc( nom, unc, header):
    hlt      = getattr( mysetting, "HLT_{}".format( opt.Year() ) )   
    c = pltmgr.NewCanvas()
    top = pltmgr.NewTopPad()
    top.Draw()
    top.cd()

    frame1 = ROOT.gPad.DrawFrame( 0, 0.6, 600, 1.6 )
    nom.Draw( "EP" )
    unc.Draw( "EP" )

    SetHist( nom, color=ROOT.kGray+1,  marker=33 )
    SetHist( unc, color=ROOT.kAzure-3, marker=8 )

    leg = pltmgr.NewLegend( 0.65, 0.6, 0.75, 0.8 )
    leg.SetHeader( header )
    leg.AddEntry( nom, "Nominal", "lp" )
    leg.AddEntry( unc, "Uncertainty", "lp" )
    leg.Draw()

    frame1.GetXaxis().SetTitle( "" )
    frame1.GetYaxis().SetTitle( "Scale factor" )
    pltmgr.SetTopPlotAxis( frame1 )

    c.cd()

    bot = pltmgr.NewBottomPad()
    bot.Draw()
    bot.cd()

    frame2 = ROOT.gPad.DrawFrame( 0, -0.055, 600, 0.055 )
    frame2.GetXaxis().SetTitle( "P_{T}" )
    frame2.GetYaxis().SetTitle( "Syst Unc" )

    pltmgr.SetBottomPlotAxis( frame2 )
    scale = pltmgr.DivideGraph( unc, nom, 1 )
    scale.Draw( "EP" )
    scale.SetLineColor( 1 )
    scale.SetMarkerColor( 1 )
    scale.SetMarkerStyle( 20 )

    line = ROOT.TLine( 0, 0, 600, 0 )
    line.SetLineColor( ROOT.kRed )
    line.SetLineStyle( 8 )
    line.Draw()

    c.cd()

    pltmgr.DrawCMSLabel( pltmgr.PRELIMINARY )
    pltmgr.DrawLuminosity( opt.Lumi() )
    pltmgr.DrawEntryLeft( hlt ) 

    c.SaveAs( "results/SF{}_pt_{}.pdf".format( opt.Year(), unc.GetName() ) )

# Initialize parsing manager
opt = parmgr.Parsemgr()
opt.Parsing()

def GetObj( file, objname ):
    temp = file.Get( objname )
    temp.SetName( objname )
    return temp

def main():


    file = ROOT.TFile.Open( "results/Eff{0}_Run20{0}_FXFX.root".format( opt.Year() ), 'read' )
    nom_pt   = GetObj( file, "pt" )
    unc1_pt  = GetObj( file, "pt_shift" )
    unc2_pt  = GetObj( file, "pt_zmass" )
    nom_eta  = GetObj( file, "eta" )
    unc1_eta = GetObj( file, "eta_shift" )
    unc2_eta = GetObj( file, "eta_zmass" )
    file.Close()

    file = ROOT.TFile.Open( "results/Eff{0}_Run20{0}_MLM.root".format( opt.Year() ), 'read' )
    unc3_pt  = GetObj( file, "pt" )
    unc3_eta = GetObj( file, "eta" )
    file.Close()
    
    PlotPtUnc ( nom_pt,  unc1_pt,  "Raise tag P_{T} threshold" )
    PlotPtUnc ( nom_pt,  unc2_pt,  "Change Zmass window" )
    PlotPtUnc ( nom_pt,  unc3_pt,  "Change MC dataset" )
    PlotEtaUnc( nom_eta, unc1_eta, "Raise tag P_{T} threshold" )
    PlotEtaUnc( nom_eta, unc2_eta, "Change Zmass window" )
    PlotEtaUnc( nom_eta, unc3_eta, "Change MC dataset" )
    


if __name__ == '__main__':
    main()
