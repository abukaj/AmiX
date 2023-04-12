# AmiX

My early dogfooding reverse engineering C(++) project.  No other software engineering techniques used.

Readme from: [Oficjalna strona AmiX-a - linuksowego klienta polchatu.](http://amix.org.pl/)
(no polish translation -sorry).

## Czym jest AmiX?

AmiX jest linuksowym klientem polchatu mojego autorstwa.  Pracuje w trybie konsolowym, wykorzystując bibliotekę ncurses.


## Screenshot (AmiX 0.2.6a rev. 13)

```
::>AmiX<:~amix_user::POLCHAT::polchat:                                            +---------- NICKS: ----------+
Pokoj uzytkownikow klienta chatu AmiX                                             |OP AmiX:AmiX v. 0.2.6a (r...|
*##################################### CHAT #####################################*|   ~AmiX_user:AmiX v. 0.2...|
00:42:25 ~AmiX_user: To na co czekamy?                                            |   ~AmiX_user_2:AmiX v. 0...|
00:42:41 ~AmiX_user_2: Na OP-a ;-D                                                |   ~AmiX_user_3:AmiX v. 0...|
00:42:56 ~AmiX_user_3: Bo bez OPa ani róż;P                                       |   ~AmiX_user_4:AmiX v. 0...|
00:43:24 ~AmiX_user_4: Tak, czekamy na AmiX-a i ewentualnych innych gości         |   ~AmiX_user_5:AmiX v. 0...|
00:43:32 ~AmiX_user_2: A będą jacyś?                                              |                            |
00:43:56 ~AmiX_user: Nom. Może wpadnie ktoś oprócz naszego sztucznego tłumu...    |                            |
00:44:18 ** przychodzi ~AmiX_user_5...                                            |                            |
00:44:33 ~AmiX_user_5: Zdążyłem?                                                  |                            |
00:46:19 ~AmiX_user: Póki nie ma OP-a, pokażcie, że nasz klient obsługuje         |                            |
polskie znaczki.                                                                  |                            |
00:46:33 ~AmiX_user_5: zażółć gęślą jaźń                                          |                            |
00:46:52 ~AmiX_user_4: ZAŻÓŁĆ GĘŚLĄ JAŹŃ                                          |                            |
00:51:06 ~AmiX_user_2: I nie tylko polskie: Нас не догонят!                       |                            |
00:51:49 ~AmiX_user: No pięknie ;P                                                |                            |
00:52:27 ~AmiX_user_2: ~AmiX_user, wpisz /lama                                    |                            |
00:52:31 ~AmiX_user: thankfully alert gauchos were able to save the llama before  |                            |
 it was swept into the blades of the turbine                                      |                            |
00:52:47 ~AmiX_user: żem wpisał, no i co?                                         |                            |
00:53:06 ~AmiX_user_2: :)                                                         |                            |
00:53:11 ~AmiX_user_3: ;)                                                         |                            |
00:53:34 ~AmiX_user_5: ;-)                                                        |                            |
00:53:42 ~AmiX_user_4: ;->                                                        |                            |
00:53:54 ~AmiX_user: a idźcie ;P                                                  |                            |
00:54:27 ** przychodzi AmiX...                                                    |                            |
00:54:36 ~AmiX_user: no wreszcie...                                               |                            |
00:55:09 ~AmiX_user_3 chce zdjęcie                                                |                            |
00:55:39 AmiX: ok, zaraz robimy                                                   |                            |
00:56:41 ~AmiX_user_2: FOTKA                                                      |                            |
                                                                                  |                            |
*################################################################################*|                            |
zrobione                                                                          +----------------------------+
```

## Skąd nazwa AmiX?

Jest to swego rodzaju hołd składany przeze mnie platformie sprzętowej Amiga - IMHO najlepszym maszynom swoich czasów (1985 - ok. 2000). X zaś ma podkreślać, ze jest to klient pod Linuxa;-).


## Licencja

1. Możesz używać tego programu w jaki sposób chcesz - ale ponosisz pełną odpowiedzialność za to, do czego go użyjesz.
2. Możesz przekazywać program dalej w postaci niezmienionego archiwum bez pobierania opłaty większej niż poniesiony koszt.
3. Możesz dowolnie modyfikować kod programu i go rozprowadzać - pod warunkiem wyraźnego zaznaczenia w dokumentacji, jakiego rodzaju zmiany wprowadziłeś.
4. Możesz wykorzystać fragmenty kodu programu w własnym programie - pod warunkiem wyraźnego i jednoznacznego zaznaczenia w dokumentacji, skąd one pochodzą.
5. Możesz okazać autorowi wdzięczność wysyłając podziękowanie, prezent (chętnie przyjmę np. wypasioną Amigę 4000, jacht bądź dajmy na to transporter opancerzony;-D).
6. Autor nie ponosi odpowiedzialności za ewentualne szkody wynikające z używania programu.

## Instalacja

Wymagania:

- Wersja najnowsza:
  - system operacyjny Linux
  - zainstalowane polecenie make (na Ubuntu pakiet _make_)
  - zainstalowany kompilator g++ (na Ubuntu pakiet _gcc-4.4-base_)
  - zainstalowana (wraz z nagłówkami) biblioteka ncurses obsługująca kodowanie utf8 (na Ubuntu pakiet _libncursesw5-dev+)
  - zainstalowana (wraz z nagłówkami) biblioteka boost obsługująca wątki (na Ubuntu pakiety _libboost-system-dev_ oraz _libboost-thread-dev_)
- Wersja 0.2.6a:
  - system operacyjny Linux
  - zainstalowane polecenie make (na Ubuntu pakiet _make_)
  - zainstalowany kompilator g++ (na Ubuntu pakiet _gcc-4.4-base_)
  - zainstalowana (wraz z nagłówkami) biblioteka ncurses obsługująca kodowanie utf8 (na Ubuntu pakiet _libncursesw5-dev_)
- Wersje do 0.2.5a włącznie:
  - system operacyjny Linux
  - zainstalowane polecenie make (na Ubuntu pakiet _make_)
  - zainstalowany kompilator gcc (na Ubuntu pakiet _gcc-4.4-base_)
  - zainstalowana (wraz z nagłówkami) biblioteka ncurses (na Ubuntu pakiet _libncurses5-dev_)


Instalacja:

1. Zainstaluj wymagane pakiety.

```commandline
$ sudo apt-get install make gcc-4.4-base libncursesw5-dev libboost-thread-dev \
libboost-system-dev
```

2. Utwórz katalog, w którym chcesz zainstalować program.

```commandline
$ mkdir ~/amix
```

3. Przejdź do utworzonego katalogu.

```commandline
$ cd ~/amix
```

4. Pobierz archiwum z najnowszą wersją programu.

```commandline
$ wget http://amix.org.pl/AmiX_0.3a1.tar.gz
```

5. Rozpakuj zawartośc archiwum.

```commandline
$ tar -xvzf AmiX_0.3a1.tar.gz
```

6. Usuń zbędne archiwum.

```commandline
$ rm AmiX_0.3a1.tar.gz
```

7. Skompiluj program.

```commandline
$ make
```

8. Zainstaluj komendę amix w systemie.

```commandline
$ sudo make install
```

7. AmiX jest już gotowy do użytku.

```commandline
$ amix
```


## Użycie

```commandline
$ amix <lista_parametrów_poprzedzonych_słowami_kluczowymi>
```


### Klawisze

| Klawisz                | Działanie              |
|------------------------|------------------------|
|Tab                     | Autouzupełnianie nicku |
|F1                      | Poprzedni pokój        |
|Shift + lewa strzałka   | Poprzedni pokój        |
| Shift + Home           | Poprzedni pokój        |
| F2                     | Następny pokój         |
| Shift + prawa strzałka | Następny pokój         |
| Shift + End            | Następny pokój         |


### Parametry

| Słowo kluczowe | Opis                                                                                                     | Wartość domyślna                                 |
|----------------|----------------------------------------------------------------------------------------------------------|--------------------------------------------------|
| NICK           | Twój nick                                                                                                | AmiX_user                                        |
| ROOM           | pokój, od którego chcesz zacząć przygodę                                                                 | AmiX                                             |
| SERVER         | serwer oferujący usługę polchat                                                                          | s1.polchat.pl                                    |
| PORT           | port na którym serwer udostępnia usługę polchat                                                          | 14003                                            |
| PERIOD         | wyrażone w sekundach opóźnienie między kolejnymi wysyłanymi wiadomościami (auto-flood-ban protection;-D) | 1.0                                              |
| NICKLISTWIDTH  | wyrażona w znakach szerokość listy nicków                                                                | 30                                               |
| NOATTR         | wyłącza atrybuty wyświetlanego tekstu                                                                    | atrybuty włączone                                |
| ASKPASSW       | wymusza zapytanie o hasło (patrz [bezpieczeństwo](#safety))                                              |                                                  | 
| CHECKUPDATES   |sprawdza po uruchomieniu programu dostępność aktualizacji	sprawdzanie aktualizacji                        | wyłączone                                        |
| LOG            | plik, do którego mają być zapisywane logi (stary plik zostanie skasowany)                                | logowanie wyłączone                              |
| OLDLOG         | plik, do którego mają być dopisywane logi (zaleca się użycie wraz z "NOHTML")                            | logowanie wyłączone                              |
| NOHTML         | oszczędniejsze logi (nie w postaci tabelki HTML)                                                         | logi HTML                                        |
| ANTIIDLE       | wyrażony w sekundach czas nieaktywności, po jakim AmiX sam wyśle pusty komunikat	"ANTIIDLE"              | wyłączone                                        |
| REFFROM        | przydatne na serwerach, które dopuszczają wejścia jedynie ze ściśle określonych stron                    | http://www.polchat.pl/chat/room.phtml/?room=AmiX |
| LATIN2         | przełącza kodowanie znaków z UTF-8 na ISO-8859-2 (Latin 2)                                               | kodowanie UTF-8                                  |
| OLD            | przywraca stary wygląd konskoli                                                                          | nowy wygląd konskoli


### Komendy

| Komenda | Opis                                                                                                                                                    |
|---------|---------------------------------------------------------------------------------------------------------------------------------------------------------|
| /borg   | wysyła tekst: "I'm cybernetic organism - living tissue over metal endoskeleton."                                                                        |
| /exit   | działanie analogiczne do polchatowego /quit, dodatkowo jednak kończy pracę programu                                                                      |
| /help   | wyświetla tekst pomocy                                                                                                                                  |
| /lama   | wysyła tekst: "thankfully alert gauchos were able to save the llama before it was swept into the blades of the turbine"                                  |
| /ver    | wyświetla numer wersji                                                                                                                                  |
| /part   | użyta w trakcie rozmowy prywatnej kończy rozmowę, użyta w pokoju działa jak normalna komenda chatu                                                      |
| /next   | przełącza aktywny pokój na następny                                                                                                                      |
| /prev   | przełącza aktywny pokój na poprzedni                                                                                                                    |
| /info   | oprócz polchatowej informacji o nicku wyświetla również nazwę klienta Polchatu, którego używa jego właściciel (o ile nick znajduje się w aktywnym pokoju) |
| /busy   | ustawia status nicka na "zajęty" (rozmową prywatną)                                                                                                      |
| /free   | ustawia status nicka na "wolny"                                                                                                                          |

Milego chatowania! ABUKAJ (J.M.Kowalski - amiga_buziaczek_pl@pomysl.zanim.napiszesz)

# AmiX a bezpieczeństwo <a name="safety"></a>

## Ogólnie

AmiX został zaprojektowany jako program bezpieczny dla użytkownika. Z tego powodu nie jest "koniem trojańskim" ani "bombą logiczną", nie zawiera niespodzianek typu backdoors (tylne wejścia) czy spyware.  Osoby znające język C mogą to sprawdzić - AmiX rozprowadzany jest w postaci kodu źródłowego.

Chociaż dołożyłem wszelkich starań, by AmiX gwarantował bezpieczeństwo, nie mogę zagwarantować, że jest on w 100% wolny od błędów, możliwych do wykorzystania przez potencjalnych krackerów.  Co więcej, groźne błędy nie muszą znajdować się w samym AmiX-ie (ale np. w oprogramowaniu TCP/IP) - dlatego nie zalecam uruchamiania AmiX-a na koncie z wysokimi uprawnieniami (np root-a).

Ponieważ crackerzy wykorzystują błędy w programach powodujące ich "dziwne" zachowanie się w pewnych szczególnych sytuacjach, wszystkie wersje AmiX-a które zachowywały się "dziwnie" w czasie testów, lub chociaż istniało **prawdopodobieństwo** że mogą się "dziwnie" zachowywać, oznaczyłem jako **NIESTABILNE**.  Nie zalecam korzystania z nich.


## Polchat 2.0 a bezpieczeństwo

Niestety, z pustego i Salomon nie naleje...  Protokół Polchat 2.0 nie jest protokołem bezpiecznym (przynajmniej w części, którą udało mi się rozpracować).  Ponieważ crackerzy i tak o tym wiedzą, zdradzę, że tekst wiadomości wysyłanych tym protokołem, jak również nicka i hasło jest w stanie odczytać każdy, kto potrafi uruchomić sniffer:-(.  Poruszyłem ten temat na forum poświęconym propozycjom ulepszeń na Polchacie. 
