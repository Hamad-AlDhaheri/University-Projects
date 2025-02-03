someData<-c(1.0456991, 1.6392089, 0.3008062, 2.3369440, 0.4728599, 1.2173219, 0.4196017, 0.1529721, 0.7154912, 2.4137436)
qqnorm(someData,main="")
qqline(someData, col="red")
hist(someData)
