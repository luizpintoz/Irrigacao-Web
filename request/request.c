#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include "request.h"

// Função para extrair o JSON da resposta HTTP
const char *extract_json(const char *response)
{
    const char *json_start = strstr(response, "\r\n\r\n"); // Encontra o final dos cabeçalhos HTTP
    if (json_start)
    {
        return json_start + 4; // Pula os 4 caracteres de "\r\n\r\n"
    }
    return NULL; // Retorna NULL se não encontrar
}

int extract_number_json()
{
    response_buffer[response_length] = '\0'; // Adiciona terminador nulo

    const char *json = extract_json(response_buffer);
    int number = 0; // Armazenar o número que contém no arquivo json
    int negative = 0;
    int size = strlen(json); // tamanho da string

    for (int i = 0; i < size; i++)
    {
        if (json[i] == '-') // Detecta se é número negativo
        {
            negative = 1;
        }
        else if (json[i] >= '0' && json[i] <= '9') // Se for um número
        {
            number = number * 10 + (json[i] - '0'); // Constrói o número corretamente
        }
    }

    if (negative)
    {
        number = -number; // Se era negativo, inverte o valor
    }

    return number;
}
static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (p == NULL)
    {

        // Conexão fechada pelo servidor
        printf("Conexão fechada pelo servidor\n");

        // Processa a resposta completa
        response_buffer[response_length] = '\0'; // Adiciona terminador nulo
        const char *json = extract_json(response_buffer);

        if (json)
        {
            printf("JSON recebido:\n%s\n", json);
        }
        else
        {
            printf("Erro: Não foi possível encontrar JSON na resposta.\n");
            printf("Resposta completa:\n%s\n", response_buffer);
        }

        tcp_close(tpcb);
        return ERR_OK;
    }

    // Copia os dados recebidos para o buffer
    int len = p->tot_len;
    if (response_length + len > RESPONSE_BUFFER_SIZE - 1)
    {
        len = RESPONSE_BUFFER_SIZE - 1 - response_length;
    }
    pbuf_copy_partial(p, response_buffer + response_length, len, 0);
    response_length += len;

    // Libera o buffer
    pbuf_free(p);

    return ERR_OK;
}

// Callback para conexão bem-sucedida
static err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if (err != ERR_OK)
    {
        printf("Erro ao conectar\n");
        return err;
    }

    tcp_recv(tpcb, tcp_recv_callback);

    // Monta requisição HTTP correta
    const char *request =
        "GET /toggle_bomba HTTP/1.1\r\n"
        "Host: " SERVER_IP "\r\n"
        "Connection: close\r\n\r\n";

    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    return ERR_OK;
}

// Função para fazer a requisição HTTP
void http_client_request()
{
    // Limpa o buffer de resposta antes de cada requisição
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    response_length = 0;

    ip_addr_t server_ip;
    ipaddr_aton(SERVER_IP, &server_ip);

    struct tcp_pcb *pcb = tcp_new();
    if (!pcb)
    {
        printf("Erro ao criar PCB\n");
        return;
    }

    if (tcp_connect(pcb, &server_ip, SERVER_PORT, tcp_connected_callback) != ERR_OK)
    {
        printf("Erro ao conectar ao servidor\n");
        return;
    }
}

void make_request()
{
    printf("Fazendo requisição à API...\n");
    http_client_request();

    // Aguarda 5 segundos antes de fazer a próxima requisição
    for (int i = 0; i < REQUEST_INTERVAL_MS / 100; i++)
    {
        cyw43_arch_poll(); // Mantém o Wi-Fi ativo
        sleep_ms(100);
    }
}

int main_request()
{
    sleep_ms(5000);

    printf("Iniciando conexão Wi-Fi...\n");

    if (cyw43_arch_init())
    {
        printf("Erro ao inicializar Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        printf("Falha ao conectar ao Wi-Fi\n");
        return 1;
    }
    printf("Conectado ao Wi-Fi!\n");

    return 0;
}