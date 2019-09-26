close all
% iss
% Benjamin Kosa
% xkosab00

% 1.)
[s, Fs]=audioread('xkosab00.wav');
% vzorkovacia frekvencia
Fs
% dlzka vo vzorkoch = pocet vzorkov
N=length(s);
N
% dlzka v sekundach
T=length(s)/Fs;
T
%pocet reprezentovanych binarn?ch symbolu
Bs=N/16;
Bs

% 2.)
ss2 = zeros(1,N/16);
c = 1;
t2 = 8:16:N/100;
t2 = t2 / 16000;
for i = 8:16:N
    if s(i) > 0
        ss2(c) = 1;
    else
        ss2(c) = 0;
    end
    c = c + 1;
end


t = linspace(0,2,32000);


figure()
plot(t(1:N/100), s(1:N/100));
hold on

stem(t2, ss2(1:N/100/16));
xlabel('t'); 
ylabel('s[n], symbol');

hold off

% 3.)
B = [0.0192 -0.0185 -0.0185 0.0192];
A = [1.0000 -2.8870 2.7997 -0.9113];

g = tf([B],[A],1/Fs);
figure();
pole(g);
pzmap(g);
%je stabilny pretoze ma poli vo vnutri kruznici

% 4.)
figure();
[mag,phase,wout] = bode(g,1:Fs/2);
bode(g,1:Fs/2)
k = find(mag<0.71,1)
wout(k)
%dolna propust
%mezni frekvence lezi v 1273hz

% 5.)
figure();
ss = lsim(g, s);
ssShifted = ss(16:end);
plot(t(1:N/100), s(1:N/100));
hold on
plot(t(1:N/100), ss(1:N/100));
hold on
plot(t(1:N/100), ssShifted(1:N/100));
%posun je 9.4*10e-4, jedna sa o zpozdeni a zistil som to od ruky

% 6.)
%figure();


% 7.)
%figure();
ssShifted2 = zeros(1,N/16);
c = 1;
for i = 8:16:N-15
    if ssShifted(i) > 0
        ssShifted2(c) = 1;
    else
        ssShifted2(c) = 0;
    end
    c = c + 1;
end

stem(t2, ssShifted2(1:N/100/16));
xlabel('t'); 
ylabel('s[n], symbol');
hold on
figure();
plot(t(1:N/100), ssShifted(1:N/100));
sumE = xor(ss2, ssShifted2);
error = sum(sumE)/length(sumE);
error
sum(sumE)


% 8.)
figure();
fourierT = fft(s); 
vector = (0:N-1)*Fs/N*2; 
module = abs(fourierT);
plot(vector(1:Fs/2), module(1:Fs/2));
xlabel('f [Hz]');
ylabel('H [z]');

figure();
fourierT = fft(ss); 
vector = (0:N-1)*Fs/N*2; 
module = abs(fourierT);
plot(vector(1:Fs/2), module(1:Fs/2));
xlabel('f [Hz]');
ylabel('H [z]');

% 9.)
figure();
hist(s, 50);

% 10.)
figure();
[Rv, lag] = xcorr(s, 'biased');
plot(lag, Rv);
xlim([-50 50]);
xlabel('k'); 
ylabel('R[k]');

% 11.)
Rv0 = Rv(0 + N);
Rv1 = Rv(1 + N);
Rv16 = Rv(16 + N);
Rv0
Rv1
Rv16


% 12.)
figure();
x = linspace(min(s), max(s), 100);
[h,p,r] = hist2(s(1:N-1), s(2:N), x);
imagesc(-x,x,p);
axis xy;
colorbar;

% 13.)
%2d integral should be 1 and is 1

%14.)
%r = 0.2327
r








