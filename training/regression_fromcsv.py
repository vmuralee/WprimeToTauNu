import keras
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
from sklearn.model_selection import train_test_split
from keras.optimizers import Adam
from keras.layers import LeakyReLU
import uproot3 as ROOT
import awkward as aw

import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-i","--inputFile",help="give the input root file to Test/Train")
parser.add_argument("-t","--train",help="to train the model",action="store_true")
parser.add_argument("-n","--nepoch",type=int,help="no. of epochs",default=100)
parser.add_argument("-m","--model",help="model name to save or load with .h5 format",default="regression_model.h5")
parser.add_argument("-s","--isSignal",help="boolean for signal",action="store_true")


args = parser.parse_args()
    

def build_model(layer_geom,learning_rate=3e-3,input_shapes=[8]):
    model = keras.models.Sequential()
    model.add(keras.layers.Flatten(input_shape=input_shapes))
    for layer in layer_geom:
        model.add(keras.layers.Dense(layer_geom[layer]))
        model.add(LeakyReLU(alpha=0.3))
        model.add(keras.layers.Dropout(0.5))
    model.add(keras.layers.Dense(1,activation='linear',kernel_initializer='normal'))
    model.compile(loss='mean_absolute_error',optimizer='adam')
    return model
    

def plot_loss(history):
    fig1,ax1 = plt.subplots()
    ax1.plot(history.history['loss'], label='loss')
    ax1.plot(history.history['val_loss'], label='val_loss')
    #ax1.set_ylim([0.5, 1.3])
    ax1.set_xlabel('Epoch')
    ax1.set_ylabel('Error [MPG]')
    ax1.legend()
    fig1.savefig('../plots/training.png')
    ax1.grid(True)




if(args.train == True):
    print(15*'==')
    TrainDataSet = pd.read_csv(args.inputFile,index_col=False)
    TrainDataSet.gen_mT.fillna(TrainDataSet.mean(),inplace=True)
    TrainDataSet.gen_dtheta.fillna(TrainDataSet.mean(),inplace=True)
else:
    TestDataSet = pd.read_csv(args.inputFile,index_col=False)
    
if(args.train == True):
    features = TrainDataSet.drop(columns=['boson_mass']).to_numpy(dtype='float32')
    labels  = TrainDataSet['boson_mass'].to_numpy(dtype='float32')
    xtrain,xtest,ytrain,ytest = train_test_split(features,labels, test_size=0.33, random_state=42)
else:
    xstar  = TestDataSet.to_numpy(dtype='float32')
    #ystar  = TestDataSet['boson_mass'].to_numpy(dtype='float32')


if(args.train):
    num_pipeline = Pipeline([
        ('std_scaler',StandardScaler()),
    ])
    xtrain = num_pipeline.fit_transform(xtrain)
else:
    num_pipeline = Pipeline([
        ('std_scaler',StandardScaler()),
    ])
    xstar = num_pipeline.fit_transform(xstar)





if(args.train == True):
    input_dim = xtrain.shape[1]
else:
    input_dim = xstar.shape[1]

hlayer_outline = {'hlayer1':64,'hlayer2':256,'hlayer3':256,'hlayer3':64}
model = build_model(hlayer_outline,input_shapes=[input_dim])
model.summary()
if args.train:
    early_stopping_cb = keras.callbacks.EarlyStopping(patience=50,restore_best_weights=True)
    checkpoint_cb = keras.callbacks.ModelCheckpoint("../models/"+args.model,save_best_only=True)
    history = model.fit(xtrain,ytrain,epochs=args.nepoch,batch_size =128,validation_data=(xtest,ytest),callbacks=[checkpoint_cb])#,early_stopping_cb])
    plot_loss(history)
else:
    model = keras.models.load_model("../models/"+args.model)

filename = 'background.root'
if args.isSignal:
    filename = 'signal.root'

if(args.train == False):
    y_pred = model.predict(xstar)
   # y_pred = num_pipeline.inverse_transform(y_pred_tr)
    fig2,ax2 = plt.subplots()
    ax2.hist(y_pred,bins=80,log=True)
    ax2.set_xlabel('invariant mass')
    ax2.set_ylabel('Events [a.u]')
    fig2.savefig('../plots/invariant_mass_signal.png')
    rootfile = ROOT.recreate(filename)
    rootfile['tree'] = ROOT.newtree({"dnn_mass":np.float32,"mT":np.float32,"dtheta":np.float32,"pTratio":np.float32})
    rootfile['tree'].extend({"dnn_mass":y_pred,"mT":TestDataSet['mT'],"dtheta":TestDataSet['dtheta'],"pTratio":TestDataSet['pTratio']})
    rootfile.close()

