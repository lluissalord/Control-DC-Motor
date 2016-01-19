function r=distance(params)
params
% (J) moment of inertia of the rotor
% (b) motor viscous friction constant
% (K) electromotive force constant 
% (R) electric resistance
% (L) electric inductance
J=params(1); b=params(2); K=params(3);
R=params(4); L=params(5);

A=[-b/J K/J; -K/L R/L];
B=[0; 1/L];
C=[1,0];
D=0;

sys=ss(A,B,C,D);

%Now we discretize the systsem with the same
%rate the data was provided
h=6.537869475138122e-03; %Extret a partir de mitja del tots els temps
sysd=c2d(sys,h);

%And now we simulate the response
global u %system input, recorded with arduino
global t %timming of the input
global y %Real output recorded with arduino
[y_model,t]=lsim(sysd,u,t);
%z=norm(y-y_model);
plot(t,y_model);
if(sum(real(eig(A)) > 0) || (J < 0 || b < 0 || K < 0 || L < 0 || R < 0))
    r=inf
else
    r=norm(y-y_model)
end