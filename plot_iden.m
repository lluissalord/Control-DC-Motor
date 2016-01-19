A=[-b/J K/J; -K/L R/L];
B=[0; 1/L];
C=[1,0];
D=0;

sys=ss(A,B,C,D);

%Now we discretize the systsem with the same
%rate the data was provided
h=6.537869475138122e-03; %Extret a partir de mitja del tots els temps
sysd=c2d(sys,h);
[y_model,t]=lsim(sysd,u,t);
set(0,'DefaultTextInterpreter', 'latex')
fontsize = 24;
set(0,'defaultaxesfontsize',fontsize);
set(0,'defaulttextfontsize',fontsize);
figure;
plot(t,v_mean(1201:length(v_mean)),'DisplayName','Resposta real')
legend
hold on;
plot(t,y_model,'DisplayName','Model identificat')
leg=legend('Resposta real','Model identificat');
set(leg,'Interpreter', 'latex');
title('Comparativa identificacio model i real');
xlabel('Temps [s]', 'Interpreter', 'latex');
ylabel('Velocitat [RPM]','Interpreter','latex');