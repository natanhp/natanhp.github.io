---
layout: ../../layouts/post.astro
title: "Supply Chain Attack: Pelajaran untuk Software Engineer"
pubDate: 2024-09-19
description: "Artikel tentang Supply Chain Attack pada lingkup software engineering."
author: "natanhp"
excerpt: "Peristiwa meledaknya pager milisi Hizbullah yang diduga karena Supply Chain Attack, membuat penulis tertarik untuk mempelajari lebih dalam lagi tentang SPA. Di sini dipaparkan pemahaman penulis tentang SPA, contoh kasus, dan cara mitigasi."
image:
  src:
  alt:
tags: ["Cyber Security"]
---

Akhir-akhir ini ramai sekali berita tentang meledaknya pager yang dipakai oleh milisi Hizbullah di Lebanon. Postingan ini tidak akan membahas konflik yang sedang terjadi, saya hanya berfokus pada skema penyerangan Supply Chain Attack (SPA) yang dilakukan. Menurut informasi yang saya baca dari GoaChronicle, diduga Israel melakukan SPA dengan cara menanamkan peledak ke pager yang akan dikirimkan ke milisi tersebut. Kasus ini yang menarik perhatian saya untuk mempelajari skema penyerangan ini, terutama di ranah saya sebagai software engineer.

Lalu apa itu SPA pada lingkup software engineering? Prinsipnya sama, di mana yang diserang adalah program atau dependency pihak ketiga dengan melakukan modifikasi berupa penanaman mallicious script, contohnya backdoor. Ketika korban menggunakan program hasil modifikasi tersebut, attacker dapat melakukan penyerangan lanjutan ke korban, misalnya backdoor yang telah ditanam dapat digunakan untuk melakukan pencurian data.

Salah satu contoh kasus yang terkenal adalah SolarWinds attack. SolarWinds adalah nama perusahaan yang mengeluarkan produk untuk kebutuhan monitoring infrastruktur. Pada 2020, salah satu produknya, yaitu Orion, disisipi backdoor oleh sekelompok hacker, yang diduga disponsori oleh negara Rusia. Akibatnya, ada sekitar 30.000 organisasi pengguna Orion yang terdampak dari serangan tersebut. 

Contoh kasus lain adalah Webmin, yang merupakan program untuk management Linux server berbasis web. Pada 2018, attacker menyusupkan backdoor pada source code yang didistribusikan lewat SourceForge. Attacker kemudian bisa menjalankan Remote Code Execution (RCE) ke korban yang menggunakan Webmin versi 1.890 dan 1.900 - 1.920. Saya menemukan labnya untuk kasus ini dan sudah saya buat write upnya, untuk memperdalam pemahaman saya mengenai bagaimana SPA bekerja dan dampaknya (Baca: [VulnHub Source: 1 Write Up](/posts/vulnhub_source_1_write_up)).

Dari kasus yang sudah saya pelajari ini, saya dapat simpulkan sebagai pengingat untuk saya sendiri ke depannya agar:
1. Selalu review code, karena di sini kita melakukan static analysis yang bisa juga untuk mendeteksi apakah program yang saya buat mengandung malicious script. Contohnya, pada source code Webmin versi 1.890 terdapat line berbahaya seperti ini:
```perl
$in{'expired'} eq '' || die $text{'password_expired'},qx/$in{'expired'}/;
```
2. Hanya gunakan program pihak ke tiga yang terpercaya.
3. Selalu up to date dengan isu keamanan terkini, terutama berkaitan dengan teknologi yang saya gunakan.
4. Jangan jalankan service langsung dengan hak akses root. Karena pada lab yang saya kerjakan, attacker tidak perlu melakukan priviledge escalation karena service berjalan dengan hak akses root.
5. Selalu pantau log. Lebih baik lagi jika menggunakan log monitoring system, yang juga dipasang alert jika ada log yang anomali. Ini juga saya pelajari sewaktu mengambil sertifikasi CCNA CyberOps, di mana saya bisa memasang Host Based Intrusion Detection System (HIDS) untuk memberikan alert ke incident response team. Pada lab yang saya kerjakan juga sebenarnya terdapat log di saat service Webmin sedang diserang. Misal pada `/var/webmin/miniserv.error` terdapat log ini ketika saya menjalankan perintah `whoami`
```bash
sh: 1: 2: not found
[18/Sep/2024:14:39:10 +0000] [192.168.75.1] /password_change.cgi : Perl execution failed : Your password has expired, and a new one must be chosen.
root
```

Pada `/var/webmin/miniserv.log` juga terdapat beberapa error 500 yang patut dicurigai
```bash
192.168.75.1 - - [18/Sep/2024:14:28:11 +0000] "POST /password_change.cgi HTTP/1.1" 500 104
192.168.75.1 - - [18/Sep/2024:14:32:08 +0000] "POST /password_change.cgi HTTP/1.1" 500 104
192.168.75.1 - - [18/Sep/2024:14:32:14 +0000] "POST /password_change.cgi HTTP/1.1" 500 3415
192.168.75.1 - - [18/Sep/2024:14:39:10 +0000] "POST /password_change.cgi HTTP/1.1" 500 109
192.168.75.1 - - [18/Sep/2024:14:42:01 +0000] "POST /password_change.cgi HTTP/1.1" 500 104
192.168.75.1 - - [18/Sep/2024:14:47:52 +0000] "POST /password_change.cgi HTTP/1.1" 500 104
192.168.75.1 - - [18/Sep/2024:14:48:18 +0000] "POST /password_change.cgi HTTP/1.1" 500 104
```
6. Jika terlanjur menggunakan program yang vulnerable mungkin bisa matikan service sambil mencari cara mitigasinya atau beralih ke program sejenis jika bisa.

Sumber:
- https://goachronicle.com/hezbollah-members-pagers-exploded-in-coordinated-attack-israel-suspected-of-hacking/
- https://www.techtarget.com/whatis/feature/SolarWinds-hack-explained-Everything-you-need-to-know
- https://www.exploit-db.com/exploits/47230