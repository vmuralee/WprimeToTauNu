# WprimeToTauNu
## Signal Sample Production
The prodction details of the **W'-> tau nu** process are given below,

Sample name | x-section <br> (pb)| no. of events 
--- | --- | --- 
WprimeM-1000ToTauNu-madgraphMLM-pythia8 | 2.457 | 500000
WprimeM-2000ToTauNu-madgraphMLM-pythia8 | 0.107 | 500000
WprimeM-3000ToTauNu-madgraphMLM-pythia8 | 0.011 | 500000
WprimeM-4000ToTauNu-madgraphMLM-pythia8 | 0.00162| 500000
WprimeM-5000ToTauNu-madgraphMLM-pythia8 | 0.00036| 500000

The main background for this analysis are the **W+ jets** and the **QCD** samples. The following table gives the main background production details.
Sample name | x-section <br> (pb)| no. of events 
--- | --- | --- 
WjetsToLNu_HT100-200| 1260 | 9751000
WjetsToLNu_HT200-400 | 255.6 | 4977207
WjetsToLNu_HT400-600 | 25.23 | 877397
WjetsToLNu_HT600-inf | 28.96 | 99799
WToLNu_M-200 | 6.772| 500000
WToLNu_M-500 | 0.2676 | 1000000
WToLNu_M-1000 | 0.0171 | 1000000
WToLNu_M-3000 | 0.0000415 | 1000000
## Tuple Producer
The analysis ntuple obtain from the delphes analysis code discussed in the ```NTupleProducer/AnalysisWprimeToTauNu.C``` file. The file contains all the training inputs and the transeverse mass of each samples. 
## DNN training 
The exclusion limits obtained from the test statics of **W'** invariant mass. The invariant mass obtained from the ML regression technique. The ML model can find at the ``` training/regression.py``` file. The inputs of the code can find by,
```
python3 training/regression.py --help
```
Once the training is performed, the model parameter saved to the ```models``` directory. While testing set ```--train``` argument into **False**