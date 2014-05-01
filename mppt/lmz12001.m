Vin = 12.6;
Vout = 5;

Rfbt = 1000;

Rfbb = 1000;

Rfbt = ((Vout/0.8)-1)*Rfbb;

ratio = ((Vout/0.8)-1);

Rfbt_E12 = 8200;
Rfbb_E12 = 1500;

ratio_E12 = Rfbt_E12/Rfbb_E12;

Vout_E12 = (ratio_E12 + 1)*0.8;

fsw = 400e3;

Ron = Vout/(1.3e-10 * fsw);

Ron_E12 = 100e3;

fsw_E12 = Vout/(1.3e-10*Ron_E12);

Ron_min = (Vin*150e-9)/1.3e-10;

fsw_max = Vout/(Vin*150e-9);

