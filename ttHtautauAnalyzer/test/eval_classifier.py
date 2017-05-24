import numpy as np
import ttHTauTauAnalysis.ttHtautauAnalyzer.mva_utils as util
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.model_selection import train_test_split

variables = """
nJet
mindr_lep0_jet
mindr_lep1_jet
avg_dr_jet
max_lep_eta
met
mht
mT_met_lep0
lep0_conept
lep1_conept
dr_leps
dr_lep0_tau
dr_lep1_tau
""".split()

print 'getting signal samples'
xsig, ysig, wsig = util.get_inputs('ttH', variables)
print 'getting background sampels'
xbkg, ybkg, wbkg = util.get_inputs('ttbar', variables)

x = np.concatenate((xsig, xbkg))
y = np.concatenate((ysig, ybkg))
w = np.concatenate((wsig, wbkg))

# ????
# negative weights due to NLO generator, for now not use weights in training
w = np.ones(x.shape[0])

# split dev, and eval samples
x_dev, x_eval, y_dev, y_eval, w_dev, w_eval = train_test_split(x, y, w,
                                                               #test_size=0.2,
                                                               # for quick test
                                                               train_size=20000,
                                                               test_size=10000,
                                                               random_state=0)
'''
#######################################################
# Grid Search
# construct classifier
bdt = GradientBoostingClassifier(verbose=0)
# parameters that will be scanned
param_grid = {"n_estimators": [200,400,1000],
              "max_depth": [1, 3, 5],
              "learning_rate": [0.02, 0.1, 0.2]},
print 'start running grid search for hyperparameters'
util.run_grid_search(bdt, x_dev, y_dev, w_dev, param_grid, verbose=1)
'''
'''
#######################################################
# Validation curve
# split dev set into train and test sample
x_train, x_test, y_train, y_test, w_train, w_test = train_test_split(x_dev, y_dev, w_dev, test_size=0.33, random_state=42)
# parameter set tuple (learn_rate, depth)
params = [(0.02,3),(0.1,5)]
clfs=[]
for rate, depth in params:
    bdt_clf = GradientBoostingClassifier(max_depth=depth,learning_rate=rate,
                                         n_estimators=1000)
    bdt_clf.fit(x_train, y_train, w_train)
    clfs.append(bdt_clf)

util.plot_validation_curve(clfs,x_train,x_test,y_train,y_test)
'''

#######################################################
# Learning curve
bdt = GradientBoostingClassifier(max_depth=3,learning_rate=0.02,n_estimators=450)
util.plot_learning_curve(bdt,"learning curve", x_dev, y_dev, scoring='roc_auc',
                         n_jobs=8, cv=4)
