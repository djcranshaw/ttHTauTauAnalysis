#!/bin/bash

outdir=${1:-/uscms/home/ztao/nobackup/ttHTT_syncNtuple/94X/30apr2018/}
inputfile=${2:-file:/uscms/home/ztao/nobackup/datasample/ttH_94X/ttHJetToNonbb.root}

#date=${1:-30apr2018}
#outdir=/uscms/home/ztao/nobackup/ttHTT_syncNtuple/94X/$date/

mkdir -p $outdir

echo 'producing sync ntuple with event selection turned off'
cmsRun analyzer2017_cfg.py doSync=True TurnOffEvtSel=True inputFiles=$inputfile
mv output_.root $outdir"output_sync.root"

makeSyncNtuple -d $outdir -o syncNtuple_object.root --makeObjectNtuple true
mv syncNtuple_object.root $outdir

echo "producing inclusive sync ntuple"
cmsRun analyzer2017_cfg.py doSync=True SampleName=sync_event_incl doCutFlow=True
mv output_sync_event_incl.root $outdir

makeSyncNtuple -d $outdir -o syncNtuple_event.root  --make1l2tau true --make2lss1tau true --make3l1tau true --make2l2tau true --makeControl true
mv syncNtuple_event.root $outdir
