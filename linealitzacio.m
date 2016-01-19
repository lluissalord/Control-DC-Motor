%PROVA
%v_mean=zeros(1,255);
%for i=1:255
%    v_mean(i)=40*log_f(i);
%end
%pwm=1:255;

%Linealitzacio
min_value=0;
index_min_value=1;
while(min_value==0)
    if(v_mean(index_min_value)>0)
        min_value=v_mean(index_min_value);
    else
        index_min_value=index_min_value+1;
    end
end

k_0=zeros(1,length(pwm));
k_c=(v_mean(length(v_mean))-v_mean(index_min_value))/(pwm(length(pwm))-pwm(index_min_value));
for i=1:length(pwm)
    k_0(i)=v_mean(i)/pwm(i);
end

K=k_c./k_0;

for i=1:255
    new_v=K*v_mean(i);
end