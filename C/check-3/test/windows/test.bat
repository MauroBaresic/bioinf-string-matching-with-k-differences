@ECHO OFF

set data_path="../../../data/Texts"

START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text5e1.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text25e1.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text1e3.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text5e3.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text1e4.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text5e4.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text1e5.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text5e5.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text1e6.fa 4
START /WAIT lvn3.exe %data_path%/pattern.fa %data_path%/text2e6.fa 4