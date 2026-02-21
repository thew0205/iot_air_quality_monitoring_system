#ifndef _LWIPOPTS_EXAMPLE_COMMONH_H
#define _LWIPOPTS_EXAMPLE_COMMONH_H


// Common settings used in most of the pico_w examples
// (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html for details)

// allow override in some examples
#ifndef NO_SYS
#define NO_SYS                      0
#endif
// allow override in some examples
#ifndef LWIP_SOCKET
#define LWIP_SOCKET                 0
#endif
#if 1//PICO_CYW43_ARCH_POLL
#define MEMP_MEM_MALLOC             1

#define MEM_LIBC_MALLOC             1
#else
// MEM_LIBC_MALLOC is incompatible with non polling versions
#define MEM_LIBC_MALLOC             0
#endif
#define MEM_ALIGNMENT               4
#ifndef MEM_SIZE
#define MEM_SIZE                    40000
#endif
#define MEMP_NUM_TCP_SEG            128
#define MEMP_NUM_ARP_QUEUE          10
/* pbuf pool */
#define PBUF_POOL_SIZE                 128
#define PBUF_POOL_BUFSIZE              512
#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_ICMP                   1
#define LWIP_RAW                    1
#define TCP_MSS                     1460
#define TCP_WND                     (8 * TCP_MSS)
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETCONN                1
#define MEM_STATS                   1
#define SYS_STATS                   0
#define MEMP_STATS                  1
#define LINK_STATS                  0
// #define ETH_PAD_SIZE                2
#define LWIP_CHKSUM_ALGORITHM       3
#define LWIP_DHCP                   1
#define LWIP_IPV4                   1
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_DNS                    1
#define LWIP_TCP_KEEPALIVE          1
#define LWIP_NETIF_TX_SINGLE_PBUF   1
#define DHCP_DOES_ARP_CHECK         0
#define LWIP_DHCP_DOES_ACD_CHECK    0

#ifndef NDEBUG
#define LWIP_DEBUG                  1
#define LWIP_STATS                  1
#define LWIP_STATS_DISPLAY          1
#endif

#define ETHARP_DEBUG                LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define PPP_DEBUG                   LWIP_DBG_OFF
#define SLIP_DEBUG                  LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF



#define TCPIP_THREAD_STACKSIZE 10000
#define TCPIP_MBOX_SIZE 10000

#define DEFAULT_TCP_RECVMBOX_SIZE 10 
#define DEFAULT_ACCEPTMBOX_SIZE 10
#define DEFAULT_UDP_RECVMBOX_SIZE 10
#define DEFAULT_RAW_RECVMBOX_SIZE 10
#define DEFAULT_THREAD_STACKSIZE 10000
#define TCPIP_MBOX_SIZE 8
#define TCPIP_THREAD_STACKSIZE 10000

#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_SNDTIMEO                1

/* netconn / sockets / api messages */
#define MEMP_NUM_NETCONN               16
#define MEMP_NUM_TCPIP_MSG_API         32
#define MEMP_NUM_TCPIP_MSG_INPKT       32

/* raw pbufs */
#define MEMP_NUM_PBUF                  64
#endif /* __LWIPOPTS_H__ */



// #ifndef _LWIPOPTS_EXAMPLE_COMMONH_H
// #define _LWIPOPTS_EXAMPLE_COMMONH_H

// /* Common settings used in most of the pico_w examples
//  * Tuned for: Raspberry Pi Pico W + LWIP + wolfSSL (AWS IoT TLS)
//  *
//  * Notes:
//  *  - This configuration assumes an environment with an OS (NO_SYS = 0).
//  *  - It gives LWIP a larger heap to accommodate TLS stacks and wolfSSL.
//  *  - If RAM is constrained, reduce MEM_SIZE and TCP buffers (see notes below).
//  */

// /* Allow override in examples/build system */
// #ifndef NO_SYS
// #define NO_SYS                      0
// #endif

// /* Enable BSD sockets (wolfSSL can use sockets or user IO; enable sockets for simpler port) */
// #ifndef LWIP_SOCKET
// #define LWIP_SOCKET                 1
// #endif

// /* If using cyw43_poll arch then use libc malloc for mem */
// #if PICO_CYW43_ARCH_POLL
// #define MEM_LIBC_MALLOC             1
// #else
// #define MEM_LIBC_MALLOC             0
// #endif

// #define MEM_ALIGNMENT               4

// /* -------------------
//  * HEAP / PBUF / MEM settings
//  * ------------------- */

// /* Increase main LWIP heap to accommodate TLS and wolfSSL allocations.
//  * Typical safe starting point for TLS + MQTT on Pico W: 64KB - 128KB.
//  * Start with 80KiB (adjust to your available RAM). */
// #define MEM_SIZE                    (80 * 1024)

// /* pbuf pool */
// #define PBUF_POOL_SIZE              64        /* number of pbufs in the pool */
// #define PBUF_POOL_BUFSIZE           512       /* size of each pbuf */

// /* memp pools - increase counts used by netconn / sockets / TCP */
// #define MEMP_NUM_PBUF               64
// #define MEMP_NUM_UDP_PCB            8
// #define MEMP_NUM_TCP_PCB            16
// #define MEMP_NUM_TCP_PCB_LISTEN     4
// #define MEMP_NUM_TCP_SEG            (32 * 2) /* 64 - fine for TLS use */
// #define MEMP_NUM_SYS_TIMEOUT        8
// #define MEMP_NUM_ARP_QUEUE          10
// #define MEMP_NUM_NETCONN            16        /* netconn/socket objects */

// /* -------------------
//  * Ethernet / ARP / IP options
//  * ------------------- */
// #define LWIP_ARP                    1
// #define LWIP_ETHERNET               1
// #define LWIP_ICMP                   1
// #define LWIP_RAW                    1
// #define LWIP_IPV4                   1

// /* -------------------
//  * TCP/UDP options (buffers & windows)
//  * ------------------- */
// /* TCP MSS typical for Ethernet */
// #define TCP_MSS                     1460

// /* Window and send buffer — TLS needs reasonably sized buffers.
//  * 16*MSS is large but helps throughput and TLS buffering. If you run out of RAM,
//  * reduce to 8*MSS. */
// #define TCP_WND                     (16 * TCP_MSS)
// #define TCP_SND_BUF                 (16 * TCP_MSS)

// /* Derived queue length for TCP send */
// #define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))

// /* Enable keepalive (useful for long-lived connections like MQTT) */
// #define LWIP_TCP_KEEPALIVE          1

// #define LWIP_TCP                    1
// #define LWIP_UDP                    1

// /* -------------------
//  * Netif / DHCP / DNS
//  * ------------------- */
// #define LWIP_NETIF_STATUS_CALLBACK  1
// #define LWIP_NETIF_LINK_CALLBACK    1
// #define LWIP_NETIF_HOSTNAME         1
// #define DHCP_DOES_ARP_CHECK         0
// #define LWIP_DHCP                   1
// #define LWIP_DNS                    1
// #define LWIP_DHCP_DOES_ACD_CHECK    0

// /* -------------------
//  * API / Sockets / Mailboxes
//  * ------------------- */
// #define LWIP_NETCONN                1

// /* Mailboxes and thread stacks - increase to allow concurrent TCP/TLS threads */
// #define TCPIP_THREAD_STACKSIZE      10000
// #define TCPIP_MBOX_SIZE             32

// #define DEFAULT_TCP_RECVMBOX_SIZE   32
// #define DEFAULT_ACCEPTMBOX_SIZE     10
// #define DEFAULT_UDP_RECVMBOX_SIZE   10
// #define DEFAULT_RAW_RECVMBOX_SIZE   32
// #define DEFAULT_THREAD_STACKSIZE    10000

// /* socket options */
// #define LWIP_SO_RCVTIMEO            1
// #define LWIP_SO_SNDTIMEO            1
// #define LWIP_NETIF_TX_SINGLE_PBUF   1

// /* -------------------
//  * Stats / Debugging
//  * ------------------- */
// /* Disable heavy stats by default to save RAM, but allow debug builds to enable */
// #define MEM_STATS                   0
// #define SYS_STATS                   0
// #define MEMP_STATS                  0
// #define LINK_STATS                  0

// #ifndef NDEBUG
// #define LWIP_DEBUG                  1
// #define LWIP_STATS                  1
// #define LWIP_STATS_DISPLAY          1
// #endif

// /* Turn on/off specific debug categories (leave most OFF for normal builds) */
// #define ETHARP_DEBUG                LWIP_DBG_OFF
// #define NETIF_DEBUG                 LWIP_DBG_OFF
// #define PBUF_DEBUG                  LWIP_DBG_OFF
// #define API_LIB_DEBUG               LWIP_DBG_OFF
// #define API_MSG_DEBUG               LWIP_DBG_OFF
// #define SOCKETS_DEBUG               LWIP_DBG_OFF
// #define ICMP_DEBUG                  LWIP_DBG_OFF
// #define INET_DEBUG                  LWIP_DBG_OFF
// #define IP_DEBUG                    LWIP_DBG_OFF
// #define IP_REASS_DEBUG              LWIP_DBG_OFF
// #define RAW_DEBUG                   LWIP_DBG_OFF
// #define MEM_DEBUG                   LWIP_DBG_OFF   /* set to ON temporarily to debug ERR_MEM */
// #define MEMP_DEBUG                  LWIP_DBG_OFF
// #define SYS_DEBUG                   LWIP_DBG_OFF
// #define TCP_DEBUG                   LWIP_DBG_OFF
// #define TCP_INPUT_DEBUG             LWIP_DBG_OFF
// #define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
// #define TCP_RTO_DEBUG               LWIP_DBG_OFF
// #define TCP_CWND_DEBUG              LWIP_DBG_OFF
// #define TCP_WND_DEBUG               LWIP_DBG_OFF
// #define TCP_FR_DEBUG                LWIP_DBG_OFF
// #define TCP_QLEN_DEBUG              LWIP_DBG_OFF
// #define TCP_RST_DEBUG               LWIP_DBG_OFF
// #define UDP_DEBUG                   LWIP_DBG_OFF
// #define TCPIP_DEBUG                 LWIP_DBG_OFF
// #define DHCP_DEBUG                  LWIP_DBG_OFF

// /* -------------------
//  * Checksum / Misc
//  * ------------------- */
// #define LWIP_CHKSUM_ALGORITHM       3

// /* -------------------
//  * Other useful options
//  * ------------------- */
// #define LWIP_NETIF_HOSTNAME         1
// #define LWIP_NETIF_STATUS_CALLBACK  1
// #define LWIP_NETIF_LINK_CALLBACK    1

// /* -------------------
//  * Notes / Tuning hints:
//  * -------------------
//  * - If you still see ERR_MEM:
//  *     - Temporarily enable MEM_DEBUG / MEMP_DEBUG to see which pool fails.
//  *     - Watch RAM usage. If MEM_SIZE is too large for your target, reduce TCP_SND_BUF
//  *       and TCP_WND (e.g. to 8 * TCP_MSS).
//  * - If using wolfSSL with its own dynamic allocations, you must ensure your application
//  *   leaves enough free heap for wolfSSL. MEM_SIZE is the pool lwIP uses when MEMP
//  *   allocations are not delegated to libc.
//  * - If MEM_LIBC_MALLOC == 1, pbuf / memp may use libc malloc; you must ensure libc malloc
//  *   has enough heap available.
//  */
// /* ---------------- DNS CONFIG REQUIRED FOR AWS + WOLFSSL ---------------- */
// #define LWIP_DNS                        1
// #define DNS_TABLE_SIZE                  4
// #define DNS_MAX_SERVERS                 3
// #define DNS_MAX_NAME_LENGTH             256
// #define MEMP_NUM_SYS_TIMEOUT            12

// /* Mailboxes required for async DNS + netconn */
// #define TCPIP_MBOX_SIZE                 32
// #define DNS_MBOX_SIZE                   32
// #define DEFAULT_TCP_RECVMBOX_SIZE       32
// #define DEFAULT_ACCEPTMBOX_SIZE         10
// #define DEFAULT_UDP_RECVMBOX_SIZE       10
// #define DEFAULT_RAW_RECVMBOX_SIZE       10

// /* async DNS mode required for netconn_gethostbyname() */
// #define LWIP_DNS_API_TYPE               LWIP_DNS_API_ASYNC

// /* End include guard */
// #endif /* _LWIPOPTS_EXAMPLE_COMMONH_H */
