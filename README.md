# Инструкция по установке нужных компонентов для использования elephant-editor

1) sudo apt install texlive-base

2) sudo apt install texlive-extra-utils

3) tlmgr repository add ftp://tug.org/historic/systems/texlive/2021/tlnet-final

4) tlmgr repository list

5) tlmgr repository remove https://mirror.ctan.org/systems/texlive/tlnet
	
	по стандарту основной репозиторий по этой ссылке, но если это не так, то пропишите 
	команду tlmgr repository remove ссылка которая при написании прошлой команды была помечена (main)
	
6) tlmgr option repository ftp://tug.org/historic/systems/texlive/2021/tlnet-final

Программа готова к использованию
