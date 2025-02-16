# Irrigação Web

Olá! Esse projeto foi desenvolvida como projeto final para o curso de sistemas embarcados do Embarcatech 👋✍️

***O CÓDIGO SÓ IRÁ FUNCIONA EM UMA PLACA BITDOGLAB!!!!!!***

### Modo de funcionamento
Clone o repositório na sua pasta desejada. 

```
git clone https://github.com/luizpintoz/Irrigacao-Web.git
```

### Instalando os requerimentos
Crie um ambiente virtual em python e instale os requerimentos.
```
python -m venv venv
venv/Scripts/activate
pip install -r requirements.txt
```

### Mudando variáveis
Mude as variáveis do Wifi na pasta "request" e a variável da pasta "thingspeak"
Por fim, inicie a aplicação flask e posteriormente compile o código e envie para placa.
```
python apiFlask/app.py
```
