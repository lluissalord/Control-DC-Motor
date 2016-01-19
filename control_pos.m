J=0.003339997274628;  b=2.892346113099947;  K=1.003140685756134;
R=0.000117295319170;  L=0.013376668730204;
A=[0 1 0; 0 -b/J K/J; 0 -K/L R/L];
B=[0; 0; 1/L];
C=[1,0,0];
D=0;

%pols de llaç tancat en continu
P=[-20,-50,-10];

%buscar controlador discret
h=0.010; %Frequencia en la que es treballarà
[phi, gam] = c2d(A,B,h);
P_dis=exp(P.*h);
K_dis=acker(phi,gam,P_dis);

%busco parametres Nu i Nx
W=[phi-eye(3) gam;C 0];
N=inv(W)*[0;0;0;1]; %abans estava [0;0;1]
Nx=N(1:3);
Nu=N(4);

phi_pert=phi;
gam_pert=gam;
C_pert=[1 0 0];

%P_obs = [-10+20i, -10-20i];
%P_obs=[-100,-150];
P_obs_dis_pert=[0, 0, 0];
K_obs_dis_pert=acker(phi_pert', C_pert', P_obs_dis_pert);
L_pert=K_obs_dis_pert';