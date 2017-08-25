
def requestName(name, useMC=False):

    if(useMC):
        return ('_' + name.split('/')[1])
    else:

        return ('_'.join( name.split('/') )).replace('-','_')    #_SingleElectron_Run2017B_PromptReco_v2_MINIAOD

def outputname(name, useMC=False):

    if(useMC):
        return '_' + (name.split('/')[1])
    else:
        return  '_' + (name.split('/')[2]).replace('-','_')

def splitbase(useMC=False):

    if(useMC):
        return 'FileBased'
    else:
        return 'LumiBased'
