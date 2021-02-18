@Echo Off
@Cls
@Echo.

Set Root=C:\

Set Tmp=%Root%Temp
Set Temp=%Tmp%
Set TmpDir=%Tmp%

Path=Z:\;%Root%Bin\Sys;%Root%Bin\Sys\DOS;%Root%Bin;Bin;\Bin;W:\Bin
Set Prompt=$P$G

Set VC=%Root%Bin\Sys\VC
Set NC=%VC%
Set BGI=%Root%Bin\BGI
Set Exe=%Root%Temp
Set DOS4G=Quiet

Keyb ru > nul
%Root%Bin\Sys\Drv\Cyr\UniKbd /ib1 /k:crs

%VC%\vc

exit
