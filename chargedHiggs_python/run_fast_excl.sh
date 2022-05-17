logdir=$1
mkdir -p logs/$logdir
cp MyConfigHplus.py logs/$logdir
#rm -r results/MyConfigHplus

#SystRankingPlot.py -p mu_SIG -f SR24_cuts -w results/MyConfigHplus/Sig_H160a24__combined_NormalMeasurement_model_afterFit.root --lumi 139
#SystRankingPlot.py -p mu_SIG -f SR24_cuts -w results/MyConfigHplus/Sig_H160a24__combined_NormalMeasurement_model_afterFit.root --input plots/ranking.data --lumi 139 --stat

#python doUL_fromLogs.py aug5_H160_unblind_fullsysobs 160 aug5_H160blind_fullsys
#python doUL_fromLogs.py aug12_H140_unblind_obsfullsys 140 aug12_H140_blind_expfullsys
#python doUL_fromLogs.py aug12_H120unblind_obsfullsys 120 aug12_H120_blind_expfullsys



#:<<END
#HistFitter.py -t -w -f -l -F excl -g H160a15 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a15.txt
HistFitter.py -t -w -f -l -F excl -g H160a16 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a16.txt
HistFitter.py -t -w -f -l -F excl -g H160a17 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a17.txt
HistFitter.py -t -w -f -l -F excl -g H160a18 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a18.txt
HistFitter.py -t -w -f -l -F excl -g H160a19 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a19.txt
HistFitter.py -t -w -f -l -F excl -g H160a20 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a20.txt
HistFitter.py -t -w -f -l -F excl -g H160a21 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a21.txt
HistFitter.py -t -w -f -l -F excl -g H160a22 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a22.txt
HistFitter.py -t -w -f -l -F excl -g H160a23 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a23.txt
HistFitter.py -t -w -f -l -F excl -g H160a24 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a24.txt
HistFitter.py -t -w -f -l -F excl -g H160a25 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a25.txt
HistFitter.py -t -w -f -l -F excl -g H160a26 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a26.txt
HistFitter.py -t -w -f -l -F excl -g H160a27 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a27.txt
HistFitter.py -t -w -f -l -F excl -g H160a28 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a28.txt
HistFitter.py -t -w -f -l -F excl -g H160a29 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a29.txt
HistFitter.py -t -w -f -l -F excl -g H160a30 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a30.txt
HistFitter.py -t -w -f -l -F excl -g H160a31 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a31.txt
HistFitter.py -t -w -f -l -F excl -g H160a32 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a32.txt
HistFitter.py -t -w -f -l -F excl -g H160a33 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a33.txt
HistFitter.py -t -w -f -l -F excl -g H160a34 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a34.txt
HistFitter.py -t -w -f -l -F excl -g H160a35 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a35.txt

HistFitter.py -t -w -f -l -F excl -g H160a36 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a36.txt
HistFitter.py -t -w -f -l -F excl -g H160a37 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a37.txt
HistFitter.py -t -w -f -l -F excl -g H160a38 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a38.txt
HistFitter.py -t -w -f -l -F excl -g H160a39 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a39.txt
HistFitter.py -t -w -f -l -F excl -g H160a40 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a40.txt
HistFitter.py -t -w -f -l -F excl -g H160a41 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a41.txt
HistFitter.py -t -w -f -l -F excl -g H160a42 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a42.txt
HistFitter.py -t -w -f -l -F excl -g H160a43 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a43.txt
HistFitter.py -t -w -f -l -F excl -g H160a44 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a44.txt

HistFitter.py -t -w -f -l -F excl -g H160a45 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a45.txt
HistFitter.py -t -w -f -l -F excl -g H160a46 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a46.txt
HistFitter.py -t -w -f -l -F excl -g H160a48 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a48.txt
HistFitter.py -t -w -f -l -F excl -g H160a50 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a50.txt
HistFitter.py -t -w -f -l -F excl -g H160a52 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a52.txt
HistFitter.py -t -w -f -l -F excl -g H160a54 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a54.txt
HistFitter.py -t -w -f -l -F excl -g H160a56 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a56.txt
HistFitter.py -t -w -f -l -F excl -g H160a58 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a58.txt
HistFitter.py -t -w -f -l -F excl -g H160a60 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a60.txt

HistFitter.py -t -w -f -l -F excl -g H160a62 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a62.txt
HistFitter.py -t -w -f -l -F excl -g H160a64 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a64.txt
HistFitter.py -t -w -f -l -F excl -g H160a66 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a66.txt
HistFitter.py -t -w -f -l -F excl -g H160a68 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a68.txt
HistFitter.py -t -w -f -l -F excl -g H160a70 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a70.txt
HistFitter.py -t -w -f -l -F excl -g H160a72 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a72.txt
HistFitter.py -t -w -f -l -F excl -g H160a74 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a74.txt
HistFitter.py -t -w -f -l -F excl -g H160a75 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a75.txt

#HistFitter.py -t -w -f -l -F excl -g H140a15 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a15.txt
#HistFitter.py -t -w -f -l -F excl -g H140a16 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a16.txt
#HistFitter.py -t -w -f -l -F excl -g H140a17 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a17.txt
#HistFitter.py -t -w -f -l -F excl -g H140a18 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a18.txt
#HistFitter.py -t -w -f -l -F excl -g H140a19 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a19.txt
#HistFitter.py -t -w -f -l -F excl -g H140a20 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a20.txt
#HistFitter.py -t -w -f -l -F excl -g H140a21 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a21.txt
#HistFitter.py -t -w -f -l -F excl -g H140a22 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a22.txt
#HistFitter.py -t -w -f -l -F excl -g H140a23 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a23.txt
#HistFitter.py -t -w -f -l -F excl -g H140a24 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a24.txt
#HistFitter.py -t -w -f -l -F excl -g H140a25 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a25.txt
#HistFitter.py -t -w -f -l -F excl -g H140a26 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a26.txt
#HistFitter.py -t -w -f -l -F excl -g H140a27 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a27.txt
#HistFitter.py -t -w -f -l -F excl -g H140a28 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a28.txt
#HistFitter.py -t -w -f -l -F excl -g H140a29 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a29.txt
#HistFitter.py -t -w -f -l -F excl -g H140a30 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a30.txt
#HistFitter.py -t -w -f -l -F excl -g H140a31 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a31.txt
#HistFitter.py -t -w -f -l -F excl -g H140a32 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a32.txt
#HistFitter.py -t -w -f -l -F excl -g H140a33 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a33.txt
#HistFitter.py -t -w -f -l -F excl -g H140a34 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a34.txt
#HistFitter.py -t -w -f -l -F excl -g H140a35 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a35.txt
#HistFitter.py -t -w -f -l -F excl -g H140a36 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a36.txt
#HistFitter.py -t -w -f -l -F excl -g H140a37 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a37.txt
#HistFitter.py -t -w -f -l -F excl -g H140a38 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a38.txt
#HistFitter.py -t -w -f -l -F excl -g H140a39 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a39.txt
#HistFitter.py -t -w -f -l -F excl -g H140a40 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a40.txt
#HistFitter.py -t -w -f -l -F excl -g H140a41 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a41.txt
#HistFitter.py -t -w -f -l -F excl -g H140a42 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a42.txt
#HistFitter.py -t -w -f -l -F excl -g H140a43 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a43.txt
#HistFitter.py -t -w -f -l -F excl -g H140a44 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a44.txt
#HistFitter.py -t -w -f -l -F excl -g H140a45 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_140a45.txt

#END 

#HistFitter.py -t -w -f -l -F excl -g H120a15 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a15.txt
#HistFitter.py -t -w -f -l -F excl -g H120a16 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a16.txt
#HistFitter.py -t -w -f -l -F excl -g H120a17 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a17.txt
#HistFitter.py -t -w -f -l -F excl -g H120a18 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a18.txt
#HistFitter.py -t -w -f -l -F excl -g H120a19 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a19.txt
#HistFitter.py -t -w -f -l -F excl -g H120a20 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a20.txt
#HistFitter.py -t -w -f -l -F excl -g H120a21 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a21.txt
#HistFitter.py -t -w -f -l -F excl -g H120a22 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a22.txt
#HistFitter.py -t -w -f -l -F excl -g H120a23 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a23.txt
#HistFitter.py -t -w -f -l -F excl -g H120a24 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a24.txt
#HistFitter.py -t -w -f -l -F excl -g H120a25 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a25.txt
#HistFitter.py -t -w -f -l -F excl -g H120a26 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a26.txt
#HistFitter.py -t -w -f -l -F excl -g H120a27 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a27.txt
#HistFitter.py -t -w -f -l -F excl -g H120a28 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a28.txt
#HistFitter.py -t -w -f -l -F excl -g H120a29 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a29.txt
#HistFitter.py -t -w -f -l -F excl -g H120a30 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_120a30.txt

#HistFitter.py -t -w -f -l -F excl -g H100a15 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_100a15.tx

#odd

#HistFitter.py -t -w -f -l -F excl -g H160a45 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a45.txt

#HistFitter.py -t -w -f -l -F excl -g H160a47 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a47.txt
#HistFitter.py -t -w -f -l -F excl -g H160a49 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a49.txt
#HistFitter.py -t -w -f -l -F excl -g H160a51 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a51.txt
#HistFitter.py -t -w -f -l -F excl -g H160a53 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a53.txt
#HistFitter.py -t -w -f -l -F excl -g H160a55 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a55.txt
#HistFitter.py -t -w -f -l -F excl -g H160a57 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a57.txt
#HistFitter.py -t -w -f -l -F excl -g H160a59 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a59.txt

#HistFitter.py -t -w -f -l -F excl -g H160a60 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a60.txt

#HistFitter.py -t -w -f -l -F excl -g H160a61 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a61.txt
#HistFitter.py -t -w -f -l -F excl -g H160a63 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a63.txt
#HistFitter.py -t -w -f -l -F excl -g H160a65 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a65.txt
#HistFitter.py -t -w -f -l -F excl -g H160a67 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a67.txt
#HistFitter.py -t -w -f -l -F excl -g H160a69 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a69.txt
#HistFitter.py -t -w -f -l -F excl -g H160a71 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a71.txt
#END
#HistFitter.py -t -w -f -l -F excl -g H160a73 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a73.txt
#HistFitter.py -t -w -f -l -F excl -g H160a75 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a75.txt

#HistFitter.py -t -w -g H160a50 -f -F excl MyConfigHplus.py | tee logs/$logdir/wspace_excl_160a50.txt
#HistFitter.py -F excl -p -l -g H160a50 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a50.txt

#HistFitter.py -t -w -g H160a55 -f -F excl MyConfigHplus.py | tee logs/$logdir/wspace_excl_160a55.txt
#HistFitter.py -F excl -p -l -g H160a55 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a55.txt

#HistFitter.py -t -w -g H160a65 -f -F excl MyConfigHplus.py | tee logs/$logdir/wspace_excl_160a65.txt
#HistFitter.py -F excl -p -l -g H160a65 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a65.txt

#HistFitter.py -t -w -g H160a70 -f -F excl MyConfigHplus.py | tee logs/$logdir/wspace_excl_160a70.txt
#HistFitter.py -F excl -p -l -g H160a70 MyConfigHplus.py | tee logs/$logdir/LimitExclFit_160a70.txt

cp MyConfigHplus.py results/MyConfigHplus

#python doUL_fromLogs.py $logdir 160  | tee logs/$logdir/limit_printout.txt
#python doUL_fromLogs.py $logdir 140  | tee logs/$logdir/limit_printout.txt
#python doUL_fromLogs.py $logdir 120  | tee logs/$logdir/limit_printout.txt


#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a40__combined_NormalMeasurement_model_afterFit.root -c SR40 -% -o logs/$logdir/SysTableSR40_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a41__combined_NormalMeasurement_model_afterFit.root -c SR41 -% -o logs/$logdir/SysTableSR41_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a42__combined_NormalMeasurement_model_afterFit.root -c SR42 -% -o logs/$logdir/SysTableSR42_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a43__combined_NormalMeasurement_model_afterFit.root -c SR43 -% -o logs/$logdir/SysTableSR43_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a44__combined_NormalMeasurement_model_afterFit.root -c SR44 -% -o logs/$logdir/SysTableSR44_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a45__combined_NormalMeasurement_model_afterFit.root -c SR45 -% -o logs/$logdir/SysTableSR45_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a46__combined_NormalMeasurement_model_afterFit.root -c SR46 -% -o logs/$logdir/SysTableSR46_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a48__combined_NormalMeasurement_model_afterFit.root -c SR48 -% -o logs/$logdir/SysTableSR48_brdown.tex

#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a60__combined_NormalMeasurement_model_afterFit.root -c SR60 -% -o logs/$logdir/SysTableSR60_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a64__combined_NormalMeasurement_model_afterFit.root -c SR60 -% -o logs/$logdir/SysTableSR64_brdown.tex

#SysTable.py -s ttbar,ttZ,Zjets -w results/MyConfigHplus_may25_160sys_ublind/Sig_H160a71__combined_NormalMeasurement_model_afterFit.root -c SR71 -% -o results/MyConfigHplus_may25_160sys_ublind/SysTableSR71_brdown.tex

#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a66__combined_NormalMeasurement_model_afterFit.root -c SR66 -% -o logs/$logdir/SysTableSR66_brdown.tex

#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a70__combined_NormalMeasurement_model_afterFit.root -c SR70 -% -o logs/$logdir/SysTableSR70_brdown.tex
#SysTable.py -s ttbar,ttZ,Zjets,ZjetsO -w results/MyConfigHplus/Sig_H160a75__combined_NormalMeasurement_model_afterFit.root -c SR75 -% -o logs/$logdir/SysTableSR75_brdown.tex

#SysTable.py -w results/MyConfigHplus/Sig_H160a57__combined_NormalMeasurement_model_afterFit.root -c SR57 -% -o results/MyConfigHplus/SysTableSR57.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a59__combined_NormalMeasurement_model_afterFit.root -c SR59 -% -o results/MyConfigHplus/SysTableSR59.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a60__combined_NormalMeasurement_model_afterFit.root -c SR60 -% -o results/MyConfigHplus/SysTableSR60.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a61__combined_NormalMeasurement_model_afterFit.root -c SR61 -% -o results/MyConfigHplus/SysTableSR61.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a63__combined_NormalMeasurement_model_afterFit.root -c SR63 -% -o results/MyConfigHplus/SysTableSR63.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a65__combined_NormalMeasurement_model_afterFit.root -c SR65 -% -o results/MyConfigHplus/SysTableSR65.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a67__combined_NormalMeasurement_model_afterFit.root -c SR67 -% -o results/MyConfigHplus/SysTableSR67.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a69__combined_NormalMeasurement_model_afterFit.root -c SR69 -% -o results/MyConfigHplus/SysTableSR69.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a71__combined_NormalMeasurement_model_afterFit.root -c SR71 -% -o results/MyConfigHplus/SysTableSR71.tex

#SysTable.py -w results/MyConfigHplus/Sig_H160a73__combined_NormalMeasurement_model_afterFit.root -c SR73 -% -o results/MyConfigHplus/SysTableSR73.tex
#SysTable.py -w results/MyConfigHplus/Sig_H160a75__combined_NormalMeasurement_model_afterFit.root -c SR75 -% -o results/MyConfigHplus/SysTableSR75.tex





