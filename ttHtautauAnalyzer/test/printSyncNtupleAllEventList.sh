#!/bin/bash

indir=${1:-./}
outdir=${2:-./}
ntuple=${3:-syncNtuple_event.root}

printEventList.sh $indir$ntuple $outidr'evlist_1l2tau_sr.dat' syncTree_1l2tau_SR
RootScanParser.sh $outidr'evlist_1l2tau_sr.dat' evlist_1l2tau_sr.txt
rm $outidr'evlist_1l2tau_sr.dat'
mv evlist_1l2tau_sr.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_1l2tau_fake.dat' syncTree_1l2tau_Fake
RootScanParser.sh $outidr'evlist_1l2tau_fake.dat' evlist_1l2tau_fake.txt
rm $outidr'evlist_1l2tau_fake.dat'
mv evlist_1l2tau_fake.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_2lss1tau_sr.dat' syncTree_2lSS1tau_SR
RootScanParser.sh $outidr'evlist_2lss1tau_sr.dat' evlist_2lss1tau_sr.txt
rm $outidr'evlist_2lss1tau_sr.dat'
mv evlist_2lss1tau_sr.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_2lss1tau_fake.dat' syncTree_2lSS1tau_Fake
RootScanParser.sh $outidr'evlist_2lss1tau_fake.dat' evlist_2lss1tau_fake.txt
rm $outidr'evlist_2lss1tau_fake.dat'
mv evlist_2lss1tau_fake.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_2lss1tau_flip.dat' syncTree_2lSS1tau_Flip
RootScanParser.sh $outidr'evlist_2lss1tau_flip.dat' evlist_2lss1tau_flip.txt
rm $outidr'evlist_2lss1tau_flip.dat'
mv evlist_2lss1tau_flip.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_3l1tau_sr.dat' syncTree_3l1tau_SR
RootScanParser.sh $outidr'evlist_3l1tau_sr.dat' evlist_3l1tau_sr.txt
rm $outidr'evlist_3l1tau_sr.dat'
mv evlist_3l1tau_sr.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_3l1tau_fake.dat' syncTree_3l1tau_Fake
RootScanParser.sh $outidr'evlist_3l1tau_fake.dat' evlist_3l1tau_fake.txt
rm $outidr'evlist_3l1tau_fake.dat'
mv evlist_3l1tau_fake.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_2l2tau_sr.dat' syncTree_2l2tau_SR
RootScanParser.sh $outidr'evlist_2l2tau_sr.dat' evlist_2l2tau_sr.txt
rm $outidr'evlist_2l2tau_sr.dat'
mv evlist_2l2tau_sr.txt $outdir

printEventList.sh $indir$ntuple $outidr'evlist_2l2tau_fake.dat' syncTree_2l2tau_Fake
RootScanParser.sh $outidr'evlist_2l2tau_fake.dat' evlist_2l2tau_fake.txt
rm $outidr'evlist_2l2tau_fake.dat'
mv evlist_2l2tau_fake.txt $outdir