# SD2016-1
##Sistemas Distribuídos 2016/1 - UFRJ - Ciência da Computação

######PARA COMPILAR


1. Vá para **_cods-aula\*/app_**, e execute os seguintes comandos:
  - make clean
  - make

2. Agora, vá para **_cods-aula\*/bin_**, e execute:
  - ./srv \<*num-porta*\>     
  - **EXEMPLO:** ./srv 2000

3. Abra outro terminal (pode ser no mesmo PC ou em outro). Vá para **_cods-aula\*/bin_** e execute:
  - ./cli \<*endereco-IP-server*\> \<*num-porta*\>
  - **EXEMPLO:** ./cli 192.168.0.14 2000
