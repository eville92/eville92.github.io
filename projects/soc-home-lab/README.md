# SOC Home Lab

## Description
A hands-on Security Operations Center lab environment with SIEM and Suricata IDS integrated on a controlled network, 
used to simulate attacks, generate logs, and develop detection rules. Four VMs will be used: a linux server, 
a Windows 11 endpoint, an SOC server, and an attackbox.

For the SIEM VM, it is recommended to have 8gb RAM (base memory: 8192 MB).

## VM Recommended Specs
### SIEM
- CPU: 4
- RAM: 8GB

### Suricata
- CPU: 4
- RAM: 8GB

### In Progress
- VM setup:
![Oracle VirtualBox Manager](assets/VMs-overview.png)

- SIEM and Suricata VMs:
![SIEM and Suricata IDS](assets/siem-suricata.png)
- [Wazuh indexer](https://documentation.wazuh.com/current/installation-guide/wazuh-indexer/installation-assistant.html) installed:
- Windows 11 setup still needed.
- Suricata installation is still required. 

## Tools & Programs
- [VirtualBox](https://www.virtualbox.org/)
- [Windows 11 Enterprise ISO](https://info.microsoft.com/ww-landing-windows-11-enterprise.html)
- [Ubuntu Server ISO](https://ubuntu.com/download/server)
- [Kali Linux](https://www.kali.org/)
- [Wazuh](https://wazuh.com/)
- [Suricata](https://suricata.io/)

## Notes
- SIEM VM froze / CPU#1 lock
- Downgraded from Ubuntu 26.04 to 22.04 for both SIEM and Suricata IDS VMs. 