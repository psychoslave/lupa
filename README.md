<p align="center">
  <img src="https://raw.githubusercontent.com/psychoslave/lupa/master/bildaro/lupa%20emblemo.png" alt="Lupa logotipo" title="Lupu vin!" />
</p>

# Lupa

**Lupa estas Lua 5.3-kongrua lingvaĵo por verki Lua-programojn per Esperanta vortprovizo.**

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

## Licenco

La projekto uzas la **MIT-licencon**.
Vidu [licenco](./LICENSE) kaj la Esperantan referencon [`PERMISILO.md`](./PERMISILO.md).
