f=fit(v_mean(82:255),pwm(82:255),'poly3');
x=min(v_mean(82:255)):max(v_mean);
plot(x,f.p1*x.^3+f.p2*x.^2+f.p3*x+f.p4)
hold on;
plot(v_mean,pwm);
hold off;
%Amb tres ja basta

%figure
%f=fit(v_mean(82:255),pwm(82:255),'poly5');
%x=min(v_mean(82:255)):max(v_mean);
%plot(x,f.p1*x.^5+f.p2*x.^4+f.p3*x.^3+f.p4*x.^2+f.p5*x+f.p6)
%hold on;
%plot(v_mean,pwm);
%hold off;