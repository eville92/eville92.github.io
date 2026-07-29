# SOC Home Lab

## Description
A hands-on Security Operations Center lab environment with SIEM and Suricata IDS integrated on a controlled network, 
used to simulate attacks, generate logs, and develop detection rules. Four VMs will be used: a linux server, 
a Windows 11 endpoint, an SOC server, and an attackbox.

## VM Specifications Used
### SIEM
- OS: Ubuntu 22.04.5 LTS
- CPU: 4
- RAM: 8GB
- Storage: 60GB

### Suricata IDS
- OS: Ubuntu 22.04.5 LTS
- CPU: 4
- RAM: 8GB
- Storage: 50GB

### Endpoint
- OS: Windows 11
- CPU: 4
- RAM: 4GB
- Storage: 60GB

### Attackbox
- OS: Kali 2026.2
- CPU: 8
- RAM: 12GB
- Storage: 50GB

## Tools & Programs
- [VirtualBox](https://www.virtualbox.org/)
- [Windows 11 Enterprise ISO](https://info.microsoft.com/ww-landing-windows-11-enterprise.html)
- [Ubuntu Server ISO](https://ubuntu.com/download/server)
- [Kali Linux](https://www.kali.org/)
- [Wazuh](https://wazuh.com/)
- [Suricata](https://suricata.io/)

## Notes
- Started project over due to Ubuntu version (26.04) issues. 
- SIEM and Suricata IDS VMs were recreated with Ubuntu 22.04 LTS. 

## In Progress
I will be updating my overall progress and issues I am having. Issues will be noted in Notes section as well. 

### Completed
- VM overview:
![Oracle VirtualBox Manager](assets/VMs-overview.png)
- SIEM & Suricata IDS VMs setup
![SIEM and Suricata IDS](assets/siem-suricata.png)
- Attackbox VM setup
![Attackbox and SSH](assets/attackbox.png)
- Wazuh and Suricata installed
![Wazuh Dashboard GUI](assets/wazuh-dashboard.png)
![Triggered Alerts](assets/wazuh-alerts.png)

### To-Do
- Windows 11 VM setup