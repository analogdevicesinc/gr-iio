
N = 2^18;
data = randn(N,1);%.*(2^15);
data2 = randn(N,1).*0;%.*(2^15);
dataC = (complex(data,data2));
save('noise.mat','dataC');

%%
