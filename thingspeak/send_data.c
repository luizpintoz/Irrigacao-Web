#include "stdio.h"
#include "lwip/tcp.h"
#include <string.h>
#include "send_data.h"

static err_t receive_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Armazena a resposta para exibição no servidor HTTP
    snprintf(thingspeak_response, sizeof(thingspeak_response), "Resposta do ThingSpeak: %.*s", p->len, (char *)p->payload);

    // Liberar o buffer e fechar conexão
    pbuf_free(p);
    tcp_close(tpcb);
    return ERR_OK;
}

// Função para enviar dados para o ThingSpeak e receber resposta
void send_data_to_thingspeak(float temperatura, float umidade, int estado_bomba)
{
    char post_data[256];

    // Formato correto: "api_key=XXXX&field1=XX&field2=XX"
    snprintf(post_data, sizeof(post_data),
             "api_key=%s&field1=%.2f&field2=%.2f&field3=%d", API_KEY, temperatura, umidade, estado_bomba);

    // Criar conexão TCP
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb)
    {
        printf("Erro ao criar PCB TCP\n");
        return;
    }

    // Endereço IP do ThingSpeak (api.thingspeak.com)
    ip_addr_t ip;
    IP4_ADDR(&ip, 184, 106, 153, 149);

    // Conectar ao servidor na porta 80
    err_t err = tcp_connect(pcb, &ip, 80, NULL);
    if (err != ERR_OK)
    {
        printf("Erro ao conectar ao ThingSpeak\n");
        tcp_close(pcb);
        return;
    }

    // Montar a requisição HTTP
    char request[512];
    snprintf(request, sizeof(request),
             "POST /update.json HTTP/1.1\r\n"
             "Host: api.thingspeak.com\r\n"
             "Connection: close\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %d\r\n\r\n"
             "%s",
             (int)strlen(post_data), post_data);

    // Enviar a requisição
    err_t send_err = tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    if (send_err != ERR_OK)
    {
        printf("Erro ao enviar os dados para o ThingSpeak\n");
        tcp_close(pcb);
        return;
    }
    else
    {
        printf("Dados enviados para ThingSpeak\n");
    }

    // Garantir que os dados sejam enviados
    tcp_output(pcb);

    // Registrar callback para receber resposta
    tcp_recv(pcb, receive_callback);
}