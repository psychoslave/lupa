# Esperantaj sinonimoj en Lupa

Tiu ĉi dokumento estas la fonto de vero pri la Esperantaj (kaj `-x`) formoj en Lupa.

- Leksilo: `fontaro/llex.c`
- Bazbiblioteko: `fontaro/lbaselib.c`
- Pakaĵa/ŝarga biblioteko: `fontaro/loadlib.c`

## Noto pri `-x` retrodefaŭlta

Kiam klavaro ne facile tajpas diakritojn, Lupa subtenas `-x` formojn (ekz. `ŝargu`/`sxargu`, `ŝirmvoku`/`sxirmvoku`).

## Leksilaj sinonimoj (`llex`)

### Ŝlosilvortoj

- `and` -> `kaj`
- `break` -> `eksterŝalte`, `ekstersxalte`, `rompe`
- `do` -> `fare`
- `else` -> `alie`

### Noto pri `~=` (neegala) kaj `zaŭ` neologismo

Antaŭe, `alie` servis kaŭ sinonimo de ambeŭ `else` (ŝlosilvortoj) kaj `~=` (operatoro). Tio kreis semantikan konflikon. Pro tio, ni revenis `alie` al eksklusiva rolo kiel `else`.

Por la operatoro `~=` (neegala), ni adoptis la neologismon **`zaŭ`** (kaj ĝiajn `-x` formon **`zaux`**) kiel mallongan alternativon al `malsamas`.

*Zaŭ* estas libere inspirata de *kvazaŭ* (= Latina *quasi*) por providi mallongan terminon. Kvankam etimologie neĉiela, ĝi ŝajnas ege taŭga opcio, konsiderante la celon de klareco kaj breveco.

- `elseif` -> `alise`
- `end` -> `hop`
- `false` -> `falsa`
- `for` -> `por`
- `function` -> `funkcie`, `tie`, `verbe`, `age`
- `goto` -> `ŝalte`, `sxalte`
- `if` -> `se`
- `in` -> `el`
- `local` -> `loka`, `loke`, `ja`
- `nil` -> `nenio`, `neo`
- `not` -> `ne`
- `or` -> `aŭ`, `aux`
- `repeat` -> `cikle`
- `return` -> `reŝalte`, `resxalte`, `ren`
- `self` (nomo de metoda ricevilo) -> `sia`
- `then` -> `tiam`
- `true` -> `vera`
- `until` -> `ĝis`, `gxis`
- `while` -> `dum`

### Operatoroj kaj signoj

- `//` -> `onige`, `parte`, `pece`, `kvociente`
- `..` -> `lige`, `kroĉe`, `krocxe`
- `...` -> `ktp`
- `==` -> `baŭ`, `egalas`, `samas`
- `>=` -> `sur`, `suras`, `almenaŭas`, `almenauxas`, `malsubas`
- `<=` -> `sub`, `subas`, `maksimumas`, `malsuras`, `malalmenaŭas`, `malalmenauxas`
- `~=` -> `zaŭ`, `zaux`, `malsamas`, `neegalas`, `nesamas`
- `~` (unuargumenta bitinverso) -> `nee`
- `~` (duargumenta bit-XOR) -> `disaŭe`, `disauxe`
- `>` -> `sup`, `super`, `superas`, `malinfraas`
- `<` -> `en`, `suf`, `infraas`, `malsuperas`
- `&` -> `kaje`
- `|` -> `aŭe`, `auxe`
- `>>` -> `sob`, `sobŝove`, `sobsxove`
- `<<` -> `sor`, `sorŝove`, `sorsxove`
- `+` -> `plus`
- `*` -> `oble`, `multiplike`
- `#` -> `pese`, `kiom`, `kvante`, `amplekse`
- `-` (unuargumenta minuso) -> `mal`, `kontraŭ`, `kontraux`
- `-` (duargumenta subtraho) -> `minus`
- `/` -> `disige`, `divide`, `ozle`
- `%` -> `laŭ`, `laux`, `module`, `kongrue`
- `^` -> `alt`, `potencige`
- `.` -> `propra`, `ie`, `ties`
- `:` -> `sin`, `ke`
- `::` -> `ho`
- `=` -> `iĝu`, `igxu`, `iĝe`, `igxe`
- `;` -> `nu`
- `,` -> `tuj`, `plie`
- `(` -> `je`
- `)` -> `ek`
- `()` (malplena alvoko) -> `lo`
- `[` -> `ĉe`, `cxe`
- `]` -> `ĉi`, `cxi`, `ere`
- `{` -> `ĉa`, `cxa`, `pri`
- `}` -> `ĉo`, `cxo`, `are`
- longĉeno-komenco (`[[`-simile) -> `cit`
- longĉeno-fino (`]]`-simile) -> `malcit`, `ĉit`
- en `cit`-ĉeno, laŭvortigi la sekvan signon -> `ĥaŭ`
- en `cit`-ĉeno, speciale interpreti eskapan komandon -> `ĥap`

La sintaksanalizilo jam distingas la kuntekstojn de `,` (ekz. disigo de esprimoj, nomlistoj, argumentoj); `tuj` kaj `plie` mapigas al la sama komo-signo.

Por `::`, la sinonimo `ho` baziĝas sur la vokativa interjekcio en Esperanto: ĝi semantike markas alvokon/alparolon al etikedo-celo (`ho etikedo ho`).

Por `()` ni aldonis la mallongigon `lo` kiel unu-vortan alvokan ŝparvojon (ekz. `io·funkcio lo`). PMEG dokumentas `lo` kiel alvokan elementon (precipe en la duvorta formo `ha lo`): <https://bertilow.com/pmeg/gramatiko/ekkrioj_sonimitoj/neoficialaj_ekkrioj.html>. Tiu noto estas konservata kiel "ene de farado" semantika klarigo por la projekta dokumentaro.

`ĉi` estis retenita por `]` (kaj ne plu por `local`) por konservi fortan leksikan paron kun `ĉe` por `[`; tio samtempe tenas klaran, simetrian legadon en proza kodo.
En abstrakta senco, `ĉi` ankaŭ estus interesa kandidato por loka markilo, sed ĝi restas rezervita ĉi tie por la ferma indeksa operatoro.

Pri la `{`/`}` paro, la formoj `ĉa`/`ĉo` resonas kun pli malnovaj reform-ideoj kiel mallongigoj de **ĉia/ĉio**; tio liveras "praneologisman" senton en la koda fluo. Referencoj: <https://www.tekstaro.com/?s=6a65e9e1bcb2d> kaj <https://www.scribd.com/document/953949064/La-Strukturo-de-Esperanto-Kiel-Faktoro-p#content=query:uzi%20la%20formojn,pageNum:131,indexOnPage:0,bestMatch:false>.

Noto pri limoj: Lua-a `[[...]]` longa ĉeno ne konflikcias kun ĉi tiu krampa sinonimaro; en Lupa la ĉenkonstruado jam havas apartan, Esperanto-amikan `cit`/`ĉit` mekanismon, do ne estas interkovro aŭ interfero inter la du subsistemoj.

Krampaj sinonimoj glutas unu apartigilan signon tuj post malfermila vorto kaj unu tuj antaŭ fermila vorto (ekz. `je¡42!ek`, `ĉa[5]ĉo`). Citiloj povas do servi kiel kadraj apartigiloj: `ĉa"saluton"ĉo` egalas al `{saluton}`, dum `ĉa""saluton""ĉo` egalas al `{"saluton"}`. La duobligitaj citiloj konservas la internajn citilojn kiel ordinaran Lua-ĉenon kaj faras la limojn simetriaj.

Por `cit`, la unua sekva **apartigila signo** (blanksigno aŭ ne-litera interpunkcio) estas ignorata, kaj same unu apartigila signo tuj antaŭ `malcit`/`ĉit` ne eniras la rezultan ĉenon. La ĉeno povas transiri plurajn liniojn ĝis `malcit` aŭ `ĉit`, por konduto pli proksima al `[[ ... ]]`.

Praktika mnemoniko por malplena ĉeno: `cit∅ĉit` (kaj ankaŭ `cit{}ĉit`) redonas `""`. Tio kongruas kun la simbola ideo de malplena aro (`∅` aŭ `{}`), samtempe montrante ke la mekanismo povas engluti unu aŭ du apartigilojn ĉe la limoj.

`ĥaŭ` en `cit`-ĉeno malaktivigas la sekvan signon por fermila detekto (ekz. `ĥaŭmalcit`, `ĥaŭĉit`, `ĥaŭĥaŭ`). La formo estis elektita ankaŭ ĉar ĝi ne aperas en Tekstaro (0 trafoj), do kolizio-risko estas minimuma.

`cit` **ne** interpretas `\`-sekvencojn (ekz. `cit \n ĉit` redonas laŭvorte `\n`). Tio estas intenca: laŭvortigo (`ĥaŭ`) kaj speciala interpreto (`ĥap`) estas apartaj mekanismoj por minimumigi surprizon.

`ĥap` estis elektita ĉar ĝi restas leksike parenca al `ĥaŭ` kaj same havas 0 trafojn en Tekstaro; tiel kolizio-risko kun ordinara Esperanto restas minimuma.

#### `ĥap`: sintakso

Du formoj estas validaj:

- **limigita formo**: `ĥape-<subkomando>-...-`
  - ekz: `ĥape-n-`, `ĥape-novlinie-`, `ĥape-x-7B-`, `ĥape-u-263A-`
- **rekta longa formo** (sen `e-`): `ĥap<plurlitera-subkomando>` aŭ `ĥap<plurlitera-subkomando>-<parametro>`
  - ekz: `ĥapspacglute`, `ĥapunikodpunkte-263A`, `ĥapdeksesume-7B`

En la limigita formo la fina `-` de la eskapo estas konsumita; la sekva signo jam apartenas al ordinara ĉena enhavo.

Intence, **rekta monoletara** formo estas nevalida: `ĥapn`, `ĥapx7B`, `ĥapz`, `ĥapu-263A`, ktp. Tio evitas konfuzon (ekz. kun `hxapx`) kaj devigas pli klarajn formojn.

Atentu: `cit` ankoraŭ forigas unu apartigilan signon tuj antaŭ `malcit`/`ĉit`. Do se eskapo produktas apartigilan signon (ekz. `{`), necesas aldoni apartan separatoron antaŭ la fermilo por ke la produktita signo restu en la rezulto (ekz. `... ĥape-x-7B- ĉit`).

#### Subkomandoj de `ĥap`

- Sen parametro:
  - `a` / `alarme`
  - `b` / `retropaŝe`
  - `f` / `paĝosalte`
  - `n` / `novlinie`
  - `r` / `ĉaretrevene`
  - `t` / `tabe`
  - `v` / `vertikalatabe`
  - `z`, `spacglute` (englutas sekvan blankspacon/novliniojn en la fonto)
  - `\`, `"`, `'` (nur en plena formo: `e-\-`, `e-"-`, `e-'-`)
  - `retrostreko`, `citilo`, `apostrofo` (legeblaj sinonimoj)
- Kun parametro:
  - `x` / `deksesume`: du deksesumaj ciferoj (0x00..0xFF), ekz. `ĥape-x-7B-` aŭ `ĥapdeksesume-7B`
  - `u` / `unikodpunkte`: unikoda kodpunkto en deksesuma formo (ĝis `10FFFF`), ekz. `ĥape-u-263A-` aŭ `ĥapunikodpunkte-263A`
  - `dekume`: 1..3 dekumaj ciferoj (0..255), ekz. `ĥape-dekume-123-` aŭ `ĥapdekume-123`

`ĥape-111-...` estas intence **nevalida**: pura cifera subkomando ne estas akceptata.

Neekzistantaj aŭ misformitaj `ĥap`-komandoj liveras eraron `invalid escape sequence`, kongrue kun Lua-stila fiasko por nevalidaj eskapoj.

### Kiam `cit` efektive malfermas ĉenon

`cit` ŝaltas ĉen-legadon nur kiam ĝi estas legata kiel aparta nomo (`TK_NAME`) kun valoro `cit`.

Praktike tio signifas:

- **Malfermas** kiam la sekva signo ne apartenas al identigilo:
  - blanksigno (`cit saluton malcit`, `cit\tsaluton malcit`, `cit\n...ĉit`)
  - interpunkcio/operatoro (`cit,saluton malcit`, `cit^¡saluton!ĉit`, `cit¡saluton!ĉit`, ktp)
- **Ne malfermas** kiam la sekva signo etendas la saman identigilon:
  - ASCII litero/cifero/substreko (`citalfa`, `cit1`, `cit_`)
  - UTF-8 liter-komenco (ekz. `citŝnuro`, `citĉeno`), ĉar tio restas unu identigilo.

Krome, `malcit`/`ĉit` fermas nur ĉe vortlimoj (do internvorta kiel `sinmalciti` aŭ `aĉiti` ne fermas).

### Klarigoj pri la realigitaj formoj

- `break` (`eksterŝalte`, `ekstersxalte`, `rompe`) estas la elireja ordono por tuj forlasi ciklon aŭ blokon; la ŝalt-derivaĵoj emfazas subitan ŝanĝon de stato.
- `function` (`funkcie`, `tie`, `verbe`, `age`) markas funkciodifinon; la proponoj celas nomi agon aŭ funkcion sen perdi la funkcio-noman rolon.
- `local` (`loka`, `loke`, `ja`) markas lokan amplekson; `ja` ne estas la plej rekta semantika analogo, sed donas mallongan prozodian markilon por emfazi la tuj sekvan lokan difinon.
- `return` (`reŝalte`, `resxalte`, `ren`) estas la kunulo de `break`: ĝi “ŝaltas reen” kun valoro aŭ sen ĝi. `ren` sekvas la jam proponitan wiki-formon, estas same mallonga kiel la antaŭa varianto, kaj restas natura Esperanto-vorto sen nova neologismo. Kompreneble la `r-` komenco ankaŭ donas mneman proksimon kun *r*eŝalte kaj *r*eturn(ig)/i.
- `==` (`baŭ`, `egalas`, `samas`) estas egaleco; la mallonga formo `baŭ` tenas la saman b/ z-simetrian ideon kiel `zaŭ`.
- `~=` (`zaŭ`, `zaux`, `malsamas`, `neegalas`, `nesamas`) estas neegaleco; `zaŭ` estas la spegulo de `baŭ`.
- `>`/`<`/`>=`/`<=` uzas la mallongajn familiojn `sup`/`sub`/`sur`/`suf` por konservi la ord-rilatan semantikon sen tro longa vortumo.
- `*` (`oble`, `multiplike`) kaj `/` (`disige`, `divide`, `ozle`) sekvas la saman lekseman simetrion kiel `baŭ`/`zaŭ`.
- `//` (`onige`, `parte`, `pece`, `kvociente`) estas la kvocienta formo de divido.
- `%` (`laŭ`, `laux`, `module`, `kongrue`) estas la resta/modula formo; `laŭ` substrekas “proporcie al”.
- `#` (`pese`, `kiom`, `kvante`, `amplekse`) mezuras la amplekson aŭ longon de la valoro.
- `^` (`alt`, `potencige`) estas la potenciga operatoro.
- `.` (`propra`, `ie`, `ties`) estas la membra aliro; `ties` donas la mallongan tabelan aliron.
- `:` (`sin`, `ke`) estas la refleksiva metodvoko; la ricevilo restas la sama objekto.
- `::` (`ho`) markas etikedon por `ŝalte`/`sxalte`.
- `...` (`ktp`) estas la variadika/“variloka” esprimo.
- `,` (`tuj`, `plie`) apartigas esprimojn, nomlistojn kaj argumentojn; la sintaksanalizilo elektas la ĝustan rolon laŭ kunteksto.
- `(` (`je`) kaj `)` (`ek`) donas la plej mallongajn prozajn krampojn; `lo` kunpremas la oftan malplenan alvokon `()`.
- `[`/`]` (`ĉe`/`ĉi`) kreas semantike klaran indekso-paron; `ere` restas pli longa ferma varianto.
- `{`/`}` (`ĉa`/`ĉo`) estas la plej mallonga tabela/ara paro; `pri`/`are` estas pli prozaj variantoj por "pri ... are" fluo.
- `;` (`nu`) estas la propozicia disigilo.

`>>`/`<<` uzas `sob`/`sor` kiel la plej mallongajn ŝovajn formojn; la pli longaj `sobŝove`/`sobsxove` kaj `sorŝove`/`sorsxove` restas pliaj stilaj variantoj.


### Notoj pri leksema simetrion

**Baŭ** por `==` (egaleco) kaj **zaŭ** por `~=` (neegaleco) kreigas etimologian simetrion:
- **`baŭ`**: Unua konsonanto de la esperanta alfabeto (b)
- **`zaŭ`**: Lasta konsonanto de la esperanta alfabeto (z)

Simile, **oble** por `*` (multiplikado) kaj **ozle** por `/` (dividado) kreigas lekseman simetrion:
- **`oble`**: Enhavas "b" (unua konsonanto) - konstruita analoge al "duplo" kaj "triplo"
- **`ozle`**: Enhavas "z" (lasta konsonanto) - konstruita kiel spegulo al "oble"

Ĉi tiu simetrio reflektas la operacian simetrion de multiplikado kaj dividado, kaj evitas kolizion kun PIV-aj vortoj.

### Notoj pri mallongaj rilatsignoj

Ĉiuj rilataj operatoroj havas almenaŭ unu mallongan (maksimume 3-signa) sinonimo:

- **`sup`** por `>` (mallongigo de *supra* = supra/super, kiu parigas bone kun *sub*). 
  Atestita en [tekstaro.com](https://tekstaro.com/t?nomo=ondo-de-esperanto&uzistreketojn=0&tipo=&sekcio=LOdE-1999-10&antauasekcio=1&postasekcio=1):
  > "SVI aperas averaĝe ĉiun trian semajnon, sup aperis kvinfoje en la jaro."
  
  Verŝajne apokopio de "supra", kaj ĝi eĉ bone respondas al "sub" laŭ terminaloga formo.

- **`sur`** por `>=` (de *supra* + direkto)
- **`sub`** por `<=` (de *sub* = sub/malsupra, kiu parigas bone kun *sup*)
- **`suf`** por `<` (mallonga membro de la sama `su-` familio; postfakta etimologia subteno eblas per formoj kiel *sufiĉa* < lat. *subfacere*, *sufikso* < lat. *subfigere*, k.a.)
- **`en`** por `<` (semantike: "3 estas *en* 5" = 3 estas *interne de* 5, ĉar 3 < 5)

## Bazbibliotekaj sinonimoj (`lbaselib`)

- `assert` -> `asertu`
- `collectgarbage` -> `senrubigu`
- `dofile` -> `dosierrulu`
- `error` -> `eraravizu`
- `getmetatable` -> `metatabelodonu`
- `ipairs` -> `induktparodonu`
- `loadfile` -> `dosierŝargu`, `dosiersxargu`
- `load` -> `ŝargu`, `sxargu`
- `loadstring` -> `ĉenŝargu`, cxensxargu`
- `next` -> `sekvdonu`
- `pairs` -> `parodonu`
- `pcall` -> `ŝirmvoku`, `sxirmvoku`
- `print` -> `printe`, `printu`
- `rawequal` -> `rektegalu`
- `rawlen` -> `rektpesu`
- `rawget` -> `rektdonu`
- `rawset` -> `rektloku`
- `select` -> `selektu`
- `setmetatable` -> `metatabelloku`
- `tonumber` -> `nombrigu`
- `tostring` -> `ĉenigu`, `cxenigu`
- `type` -> `tipdonu`
- `xpcall` -> `traktŝirmvoku`, `traktsxirmvoku`

## Pakaĵaj kaj ŝargaj sinonimoj (`loadlib`)

- `loadlib` -> `tekŝargu`, `teksxargu`
- `searchpath` -> `dosierindikosercxu`, `dosierindikoserĉu`
- `seeall` -> `plenvidu` (nur en `LUA_COMPAT_MODULE`)
- `require` -> `necesu`

## Testa kovrado

La rilataj testoj troviĝas en:

- `testaro/sinonimoj-leksilo.lupa`
- `testaro/sinonimoj-bibliotekoj.lupa`
- `testaro/cit-kazoj.lupa`
- `testaro/lua53-kongruo.lupa`
- `testaro/lotpocio-kongruo.sh`

La tria testdosiero provas kondutan kongruon inter Lua 5.3 kaj Lupa por kanona Lua-kodo (sen Lupa-specifaj sinonimoj).
