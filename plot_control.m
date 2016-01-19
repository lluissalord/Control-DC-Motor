function plot_control(X,Y,title_plot,names, x_min, x_max, y_min, y_max)

set(0,'DefaultTextInterpreter', 'latex')

fontsize = 24;
set(0,'defaultaxesfontsize',fontsize);
set(0,'defaulttextfontsize',fontsize);

figure
hold on;

for i=1:length(Y(1,:))
plot(X,Y(:,i),'DisplayName',names(i));
end

title(title_plot);
xlabel('Temps [s]', 'Interpreter', 'latex');
ylabel('Velocitat [RPM]','Interpreter','latex');

axis([x_min x_max y_min y_max]);

h=legend(names);
set(h,'Interpreter', 'latex');

grid on;

end

%example
%plot_control(time,[Ref,U,V1,V2,X1,X2],'Control amb observador',char('$$Ref$$','$$U$$','$$V_1$$','$$V_2$$','$$X_1$$','$$X_2$$') ,0,25,-2.5,1.5)