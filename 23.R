prob<-c(0.54, 0.38, 0.06, 0.02)
X<-c(1, 2, 3, 4)
X*prob
EXP_X<-(sum(X*prob))
EXP_X
E_X.sq<-(sum(X^2*prob))
E_X.sq-EXP_X^2
