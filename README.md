<p align="center">
  <img src="https://raw.githubusercontent.com/psychoslave/lupa/master/bildaro/lupa%20emblemo.png" alt="Lupa logotipo" title="Lupu vin!" />
</p>

# Lupa

**Lupa celas retrokongruecon kun Lua 5.3, sed liveras superaron de ebloj: pli vasta Unikoda subteno kaj nova ĉen-konstruaĵo per la unika `citĉit-ilo`.**

Lupa estas disbranĉiĝo de Lua 5.3.3 kun Esperantaj sinonimoj por ŝlosilvortoj, operatoroj kaj parto de la normaj bibliotekoj.

Frata projekto: [Mallupa](https://github.com/psychoslave/mallupa), kiu tradukas dialektojn al kanona Lua-kodo.

## Antaŭkondiĉoj

- `make` (uzata tra `./fare`)
- C-kompililo (`gcc` aŭ `cc`)
- bazaj iloj: `ar`, `ranlib`, `rm`
- por iuj platformoj: `readline`

## Konstruado

```bash
# aŭtomata platformdetekto
./fare aŭtokonstrui

# mane elekti celotan platformon
./fare linux
./fare macosx
./fare freebsd
```

## Testado

```bash
# plena testaro (ĉiuj .lupa, .lua kaj .sh en testaro/)
./fare plentesti

# rapida kontrolo de interpretilo
./fare testi
```

## Instalado

```bash
# sisteme (defaŭlta prefikso: /usr/local)
sudo ./fare instali

# loka instalado por provado (en ../instali)
./fare lokali

# malinstali
sudo ./fare malinstali
```

## Tipaj ordonoj (`./fare ...`)

| Ordo | Kion ĝi faras |
| --- | --- |
| `./fare aŭtokonstrui` | Detektas sistemon kaj konstruas per taŭga platforma celo |
| `./fare plentesti` | Konstruas kaj rulas la tutan testaron |
| `./fare testi` | Rulas rapidan versian kontrolon de `lupe` |
| `./fare instali` | Instalas duumaĵojn, inkluzivaron, bibliotekon kaj manpaĝojn |
| `./fare lokali` | Instalas loke sub `../instali` |
| `./fare malinstali` | Forigas instalitajn dosierojn |
| `./fare eĥi` | Montras la aktivajn agordajn parametrojn |

## Subtenataj platformoj kaj limigoj

Subtenataj platformaj celoj en `fareblaro`:
`aix`, `bsd`, `c89`, `freebsd`, `generic`, `linux`, `macosx`, `mingw`, `posix`, `solaris`.

Gravaj notoj:

- `aŭtokonstrui` aŭtomate mapigas al `macosx`, `linux`, `freebsd`, `mingw` aŭ `posix`.
- En iuj sistemoj necesas `readline` por ligi la interpretilon.
- `c89` ekzistas por kongrueco, sed ne garantias 64-bitajn entjerojn.
- `mingw` kreas `.exe`-duumaĵojn laŭ la tradicia Lua-fluo.

## Lua kontraŭ Lupa (flank-ĉe-flanke)

Jen pli reala ekzemplo: malantaŭenspura **N-reĝina serĉado**, fame popularigita en la Lua-komunumo.

En la maldekstra flanko, la Lua-kodo restas kun tute anglaj identigiloj.  
En la dekstra flanko, la Lupa-kodo montras Esperantajn identigilojn kun la meza punkto (`·`) por kunmetitaj nomoj.  
Tio estas **eblo, ne devigo**: vi povas uzi ankaŭ simplajn identigilojn laŭ via prefero.

<div style="display: flex; flex-wrap: wrap; gap: 1rem; align-items: flex-start;">
  <div style="flex: 1 1 24rem; min-width: 20rem;">
    <strong>Originala Lua</strong>
<pre><code class="language-lua">local function is_safe(row_index, column_index, queen_positions)
  for previous_row = 1, row_index - 1 do
    local previous_column = queen_positions[previous_row]
    if previous_column == column_index then
      return false
    end
    if math.abs(previous_column - column_index) == (row_index - previous_row) then
      return false
    end
  end
  return true
end

local function place_queens(row_index, board_size, queen_positions)
  if row_index > board_size then
    return true
  end
  for column_index = 1, board_size do
    if is_safe(row_index, column_index, queen_positions) then
      queen_positions[row_index] = column_index
      if place_queens(row_index + 1, board_size, queen_positions) then
        return true
      end
    end
  end
  return false
end
</code></pre>
  </div>
  <div style="flex: 1 1 24rem; min-width: 20rem;">
    <strong>Lupa-adaptaĵo</strong>
<pre><code class="language-lua">loka funkcio estas·sekura(vico·indekso, kolumno·indekso, reĝino·pozicioj)
  por antaŭa·vico = 1, vico·indekso - 1 fare
    loka antaŭa·kolumno = reĝino·pozicioj[antaŭa·vico]
    se antaŭa·kolumno == kolumno·indekso tiam
      reŝalte falsa
    hop
    se matematiko.abs(antaŭa·kolumno - kolumno·indekso) == (vico·indekso - antaŭa·vico) tiam
      reŝalte falsa
    hop
  hop
  reŝalte vera
hop

loka funkcio metu·reĝinojn(vico·indekso, tabulo·grando, reĝino·pozicioj)
  se vico·indekso > tabulo·grando tiam
    reŝalte vera
  hop
  por kolumno·indekso = 1, tabulo·grando fare
    se estas·sekura(vico·indekso, kolumno·indekso, reĝino·pozicioj) tiam
      reĝino·pozicioj[vico·indekso] = kolumno·indekso
      se metu·reĝinojn(vico·indekso + 1, tabulo·grando, reĝino·pozicioj) tiam
        reŝalte vera
      hop
    hop
  hop
  reŝalte falsa
hop</code></pre>
  </div>
</div>

## Sinonimoj kaj fallback

Detala dokumentado pri Esperantaj sinonimoj (inkluzive `-x` fallback-formoj) troviĝas en:

- `dokumentaro/esperantaj-sinonimoj.md`

## Kontribuado

Kontribuoj estas bonvenaj per:

- Eldonoj: <https://github.com/psychoslave/lupa/issues>
- Tirpetoj: <https://github.com/psychoslave/lupa/pulls>

## Agnoskoj

Ni volas esprimi nian profundan dankon al ĉiuj kiuj kontribuis al ĉi tiu projekto, ĉu per rektaj kodkontribuoj, ĉu per iloj kaj servoj kiuj ebligis ĝian evoluon.

### Personoj

**Pradifuzintoj de Esperanto:**
- [L. L. Zamenhof](https://eo.wikipedia.org/wiki/L._L._Zamenhof) — Kreinto de Esperanto
- [Antoni Grabowski](https://eo.wikipedia.org/wiki/Antoni_Grabowski) — Pioniro de Esperanto, kreinto de la vorto "ren"

**Esperanto komunumo:**
- [Espéranto-France Strasbourg](https://strasbourg.esperanto-france.org/) — Loka Esperanto-klubo en Strasburgo, kiu enkondukis la aŭtoron de Lupa al flua scio de Esperanto

**Kreintoj de Lua (PUC-Rio, Brazilo):**
- [Roberto Ierusalimschy](https://www.inf.puc-rio.br/~roberto/) — Ĉefa arkitekto de Lua
- [Waldemar Celes](https://www.tecgraf.puc-rio.br/~celes/) — Kun-kreinto de Lua
- [Luiz Henrique de Figueiredo](https://www.tecgraf.puc-rio.br/~lhf/) — Kun-kreinto de Lua

### Serviloj

C̄i tiuj servoj gravas por la evoluo kaj testado de la projekto:

| Servo | Priskribo | Ligilo |
|-------|-----------|--------|
| <img src="https://komputeko.net/Komputeko2012.png" alt="Komputeko" width="16"> **Komputeko** | Esperanta komputila vortaro | [komputeko.net](https://komputeko.net) |
| <img src="https://vortaro.net/favicon.ico" alt="Vortaro" width="16"> **Vortaro** | Reta Esperanta vortaro | [vortaro.net](https://vortaro.net) |
| <img src="https://reta-vortaro.de/favicon.ico" alt="Reta Vortaro" width="16"> **Reta Vortaro** | Esperanta-Germana/Germana-Esperanta vortaro | [reta-vortaro.de](https://reta-vortaro.de) |

### Aliaj iloj

C̄i tiuj iloj estis uzataj dum la evoluo de ĉi tiu projekto:

| Ilo | Priskribo | Ligilo |
|-----|-----------|--------|
| **CLion** | Integra evolu-medio | [jetbrains.com/clion](https://www.jetbrains.com/clion/) |
| **Fish-shell** | Amika komando-Ŝelo | [fishshell.com](https://fishshell.com) |
| **GNU Coreutils** (bash, cat, cd, head, sort, uniq, etc.) | Bazaj komandoj de Unikso | [gnu.org](https://www.gnu.org/software/coreutils/) |
| <img src="https://git-scm.com/favicon.ico" alt="Git" width="16"> **Git** | Versia kontrolo | [git-scm.com](https://git-scm.com) |
| <img src="https://github.githubassets.com/favicons/favicon-dark.png" alt="GitHub" width="16"> **GitHub** | Koda gastigado kaj kunlaboro | [github.com](https://github.com) |
| <img src="https://cli.github.com/favicon.png" alt="GitHub CLI" width="16"> **GitHub CLI (gh)** | Komandlinia interagado kun GitHub | [cli.github.com](https://cli.github.com) |
| **curl** | Transigo de datumoj per URL | [curl.se](https://curl.se) |
| **grep** | Serĉado en tekstoj | [gnu.org](https://www.gnu.org/software/grep/) |
| **Mistral Vibe** | Koda helpilo uzata dum evoluo | [mistral.ai](https://mistral.ai) |
| **ripgrep (rg)** | Rapida serĉado en tekstoj | [github.com/BurntSushi/ripgrep](https://github.com/BurntSushi/ripgrep) |
| **Vim** | Teksta redaktilo | [vim.org](https://www.vim.org) |

### Juridika notigo pri iloj

> **Kial ni ne agnoskas ilojn kiel kunverkintojn?**
> 
> Ni profundse dankas al ĉiuj iloj kiuj helpis la evoluon de ĉi tiu projekto. Tamen, el juridika vidpunkto, **nek unue juĝejoj agnoskas aŭtorrajtojn por programariloj**, sendepende de ilia potenco aŭ utileco. Aŭtorrajto estas rezervita por homaj kreintoj. Tial, ni decidis ne inkluzivi ilojn kiel "Co-Authored-By" en niaj git-komitmesaĝoj. Iloj restas valoraj partneroj en la evoluprocezo, sed ne juraj kunverkintoj.

## Licenco

La projekto uzas la **MIT-licencon**.
Vidu [licenco](./LICENSE) kaj la Esperantan referencon [`PERMISILO.md`](./PERMISILO.md).
