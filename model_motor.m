function f=model_motor(w_data,V_data,i,bJ,KJ,KL,RL,L)
%S'utilitzen els termes de les matrius i la variable i, per tal de
%facilitar el càlcul, ja que sino eren tot termes no lineal

w_model=zeros(1,length(w_data));
w_model(1)=w_data(1);
i_model=zeros(1,length(w_data));
i_model(1)=i(1); %Deixem que esculli la optimitzacio o hauria de ser i=(V-K*w_data(1))/R ???
for t=2:length(w_data)
    w_model(t)=w_model(t-1) - bJ*w_model(t-1) + KJ*i(t-1);
    i_model(t)=i_model(t-1) - KL*w_model(t-1) - RL*i(t-1) + V_data(t-1)/L;
end

f=sum(abs(w_data-w_model))+sum(abs(i-i_model));

end