# Structuri de Date - Tema 2

## Structuri de date

- Pentru tema aceasta am folosit 3 structuri de date predate la curs:
  1. Graf cu lista de adiacenta (`ListGraph`)
  2. Dictionar (`Dictionary`)
  3. Arbore binar de cautare (`BinarySearchTree`)
- Implementarile sunt generice si se gasesc in `src/DataStructures/`

### Dictionar

#### Implementare

- Implementarea este generica si trebuie specificat tipul `T` de date care va
  fi stocat si o functie `Hash` pentru tipul `T`.
- Datele sunt stocate in vectorul `m_Buckets` in functie de valoarea returnata
  de functia `Hash`.
- Dimensiunea vectorului `m_Buckets` poate fi specificata in constructorul
  dictionarului, altfel se va alege o valoare predefinita (un numar prim).
- Fiecare `bucket` este o lista inlantuita (`std::list`) de tine date tip `Node`.
- Un `Node` contine: cheia si valoarea asociata.
- Pe langa functiile de baza ale unui dictionar, am adaugat `operator[key]` ce
  returneaza o referinta la cheia `key`. Daca nu se gaseste cheia in dictionar,
  atunci aceasta se adauga in dictionar cu valoarea implicita si se returneaza
  o referinta.

#### Utilizare in cadrul temei

- Pentru tema, am folosit dictionarul in cadrul structurii `ListGraph` cu
  scopul de a asocia eticheta unui nod (`std::string`) cu ID-ul nodului (`int`).
- In fisierul `src/Utils.h` am definit o functie de hash pentru tipul `std::string`.

### Graf cu liste de adiacenta

#### Implementare

- Pentru declararea unui `ListGraph` trebuie specificat tipul `T` al nodurilor
  si o functie `Hash` pentru tipul `T` intrucat se utilizeaza un `Dictionar`.
- Dictionarul este utilizat pentru asocierea unei etichete de nod (label) cu un
  ID deoarece intern, structura nu lucreaza direct cu nodurile declarate de
  utilizator (care pot avea orice tip) pentru ca ar fi ineficient, ci lucreaza
  cu aceste ID-uri care iau valori intre `0` si `size`, unde `size` este numarul
  de noduri.
- Fiecare functie ce se apeleaza asupra grafului primeste ca parametri noduri de
  tip `T`, iar acestea sunt convertite intern la ID-ul aferent, iar de aici se
  utilieaza functiile clasice pentru grafuri care lucreaza cu noduri de tip numar
  (adaugare/stergere/verificare muchii, BFS, listare vecini).
- Determinarea distantei dintre noduri se face cu BFS.
- Structura permite precalcularea tuturor distantelor si salvarea intr-o matrice
  pentru o gasire rapida. Daca se modifica graful, atunci matricea se invalideaza
  automat.
- Daca matricea de distante este invalida, atunci distanta dintre noduri se
  calculeaza pe loc.

#### Utilizare in cadrul temei

- Graful este folosit pentru salvarea hartii rutiere a orasului.
- Nodurile sunt de tip `std::string` si reprezinta numele intersectiilor,
  iar muchiile reprezinta strazile care le leaga.
- Functia de hash este implementata in `src/Utils.h`.
- Distanta dintre doua intersectii este reprezentata de numarul minim de
  muchii ce trebuie parcurse pentru a ajunge din A in B.
- Deci, distanta dintre A si B se calculeaza cu un BFS din nodul A.
- Pentru a verifica daca exista legatura intre doua intersectii, verific
  distanta dintre ele. Daca nu se poate calcula distanta, atunci nu exista.
- Dupa ce graful nu se mai modifica (la sfarsitul task-ului 3), precalculez
  toate distantele din graf pentru a nu mai le calcula la fiecare interogare.
  (aplic BFS in fiecare nod)

### Arbore binar de cautare

#### Implementare

- Implementarea este generica si trebuie specificat tipul `T` de date care va
  fi stocat si o functie `Compare(a, b)` pentru tipul `T` care returneaza `true`
  daca si numai daca `a < b`.
- Functia de sortare este folosita pentru a determina ordinea elementelor in
  arbore.
- Pe langa functiile elementare (insert, remove, exists) a fost implementata
  o functie de parcurgere a arborelui care returneaza un `std::vector<T>`
  continand elementele in ordine.

#### Utilizare in cadrul temei

- La task-ul 4 pentru realizarea celor 3 topuri (rating mediu, distanta totala,
  numar total de curse).
- Functiile de comparare ale soferilor pentru cele 3 topuri sunt definite in
  fisierul `src/DriverPool.h`
- La fiecare modificare a statisticilor unui sofer, adica la preluarea unei
  curse, acesta este sters din si readaugat in arbori pentru a ajunge pe
  pozitia corecta.
- Arborele pentru rating-ul mediu este utilizat si pentru gasirea celui mai bun
  sofer pentru o anumita locatie deoarece avand toti soferii sortati dupa rating
  si nume, inseamna ca primul sofer cu distanta minima la locatie este si cel
  care satisface criteriile de rating maxim si nume minim dpdv lexicografic.
- Un arbore mai este utilizat la task-ul 5 pentru a memora combustibilul ramas
  pana la fiecare locatie data.
- Proprietatea de unicitate a datelor este de ajutor aici deoarece locatiile se
  pot repeta la citirea din fisier, insa acestea nu pot fi introduse in arbore
  de mai multe ori, deci la afisare nu va fi nicio dublura.
- Functia de sortare pentru acest caz este definita in cadrul structurii
  `RemainingFuel` ca `operator<`.

## Clase aditionale

### `Driver`

- Aici sunt stocate toate datele despre un sofer (nume, locatie, distanta etc)
- Implementeaza toate operatiile ce pot fi aplicate unui sofer, printre care:
  - `getDistanceTo`: calculeaza distanta pana la o anumita locatie
  - `driveTo`: seteaza locatia si incrementeaza contorul pentru distanta parcursa
  - `pickupRider`: incrementeaza contorul pentru numarul de curse
  - `(g/s)etStatus`: control pentru status

### `DriverPool`

- Clasa utilizata pentru memorarea tuturor soferilor (intr-un vector, probabil
  ar fi fost mai optim un dictionar, dar numarul de soferi este relativ mic,
  deci nu s-ar simti vreo diferenta).
- Implementeaza operatii care se aplica asupra tuturor soferilor, printre care:
  - `insertDriver`: adauga un nou sofer
  - `getDriver`: gaseste soferul cu un anumit nume
  - `getBestDriver`: gaseste cel mai bun sofer pentru o anumita locatie in
  functiile de criteriile specificate de cerinta
  - functii de listare a soferilor in functie de anumite criterii (distanta etc)
- Pe langa memorarea tuturor soferilor, clasa mai implementeaza 3 arbori binari
  de cautare in care sunt stocati pointeri la toti soferii, ordinati dupa cele 3
  criterii pentru cele 3 topuri.
- Tot in cadrul acestei clase sunt implementate functiile de sortare pentru
  arborii de mai sus.

## Biblioteca `Utils.h`

- Nu stiu daca este permisa utilizarea a `std::hash` si `std::less`, deci am
  inclus aici o implementare de hash pentru `std::string` (doar de asta e
  nevoie la tema) si o reimplementare simpla pentru `std::less`.

## Probleme intalnite

- Task-ul 4 - de mai multe ori:
  - initial, pentru aflarea unui top, sortam toti soferii dupa criteriul necesar
    ceea ce s-a dovedit ineficient pe ultimele teste, chiar si cu `std::sort`
  - dupa, am incercat un `std::partial_sort` si sortam doar atati soferi cati
    imi trebuiau pentru top, insa tot picau unele teste.
  - am creat 3 `std::set`-uri de soferi pentru fiecare top. Acum treceau testele,
    dar cerinta nu imi permite sa folosesc `std::set`, dar macar am gasit solutia
  - am facut downgrade de la RBTree la BST si am vazut ca inca trece testele,
    deci am ramas la solutia asta.
- vmchecker: testele trec pe local, dar pica pe vmchecker la testul 4.
  - dupa multe verificari ale codului, am verificat si pe ce ruleaza vmcheckerul,
    unde mi-a sarit in ochi faptul ca era folosit un kernel x86. Am luat versiunea
    de Ubuntu si versiunea compilatorului si am creat o masina virtuala care simula
    vmcheckerul, pe arhitectura x86. Acum imi pica testele si pe local.
  - dupa alte multe verificari ale codului, am vazut ca problema era la compararea
    rating-ului mediu, care este un `float` rezultat din impartirea a doua numere
    intregi. Am incercat sa trec la `double`, dar tot picau pe vmchecker.
  - solutie: aduc la acelasi numitor si compar ca numere intregi.
  - problema: tratarea separata a cazului cand numitorul e 0
- Propunere: sa se posteze undeva informatii hardware + software despre masinile
  pe care se verifica temele.
- Propunere 2: sa se poata descarca output-ul testelor sau macar output-ul de la
  stdout (momentan este o limita la ce se poate afisa la stdout, tot ce este peste
  x caractere nu mai este afisat in browser)
