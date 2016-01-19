J=0.003339997274628;  b=2.892346113099947;  K=1.003140685756134;
R=0.000117295319170;  L=0.013376668730204;
A=[-b/J K/J; -K/L R/L];
B=[0; 1/L];
C=[1,0];
D=0;

%pols de llaç tancat en continu
P=[-100,-150];

%buscar controlador discret
h=0.010; %Frequencia en la que es treballarà
[phi, gam] = c2d(A,B,h);
P_dis=exp(P.*h);
K_dis=acker(phi,gam,P_dis);

%busco parametres Nu i Nx
W=[phi-eye(2) gam;C 0];
N=inv(W)*[0;0;1]; %abans estava [0;0;1]
Nx=N(1:2);
Nu=N(3);

phi_pert=[phi gam; 0 0 1];
gam_pert=[gam; 0];
C_pert=[1 0 0];

%P_obs = [-10+20i, -10-20i];
%P_obs=[-100,-150];
P_obs_dis_pert=[0, 0, 0.9];
K_obs_dis_pert=acker(phi_pert', C_pert', P_obs_dis_pert);
L_pert=K_obs_dis_pert';