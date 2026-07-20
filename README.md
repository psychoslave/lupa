<p align="center">
  <img src="https://raw.githubusercontent.com/psychoslave/lupa/master/bildaro/lupa%20emblemo.png" alt="Lupa logotipo" title="Lupu vin!" />
</p>

# Lupa

**Lupa estas Lua 5.3-kongrua lingvaĵo por verki Lua-programojn per Esperanta vortprovizo.**

Lupa estas disbranĉiĝo de Lua 5.3.3 kun Esperantaj sinonimoj por ŝlosilvortoj, operatoroj kaj parto de la normaj bibliotekoj.

Frata projekto: [Mallupa](https://github.com/psychoslave/mallupa), kiu tradukas dialektojn al kanona Lua-kodo.

## Rapida komenco

### Antaŭkondiĉoj

- `make` (uzata tra `./fare`)
- C-kompililo (`gcc` aŭ `cc`)
- bazaj iloj: `ar`, `ranlib`, `rm`
- por iuj platformoj: `readline`

### Minimumaj paŝoj por ekkuri

```bash
# 1) konstrui (aŭtomata platformdetekto)
./fare aŭtokonstrui

# 2) rapida kontrolo
./fare testi

# 3) ruli ekzemplon
./fontaro/lupe ekzemplaro/saluti.lupa
```


## Konstruado

```bash
# aŭtomata platformdetekto
./fare aŭtokonstrui

# mane elekti platformon
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

## Lua kontraŭ Lupa (flank-al-flanke)

Jen pli reala ekzemplo: malantaŭenspura **N-reĝina serĉado**, fame popularigita en la Lua-komunumo.

<table>
  <tr>
    <th>Originala Lua</th>
    <th>Lupa-traduko</th>
  </tr>
  <tr>
    <td>
<pre><code class="language-lua">local function estas_sekura(vico_posteno, kolumno_posteno, pozicioj)
  for antaŭa_vico = 1, vico_posteno - 1 do
    local antaŭa_kolumno = pozicioj[antaŭa_vico]
    if antaŭa_kolumno == kolumno_posteno then
      return false
    end
    if math.abs(antaŭa_kolumno - kolumno_posteno) == (vico_posteno - antaŭa_vico) then
      return false
    end
  end
  return true
end

local function metu_reĝinojn(vico_posteno, tabulo_grando, pozicioj)
  if vico_posteno > tabulo_grando then
    return true
  end
  for kolumno_posteno = 1, tabulo_grando do
    if estas_sekura(vico_posteno, kolumno_posteno, pozicioj) then
      pozicioj[vico_posteno] = kolumno_posteno
      if metu_reĝinojn(vico_posteno + 1, tabulo_grando, pozicioj) then
        return true
      end
    end
  end
  return false
end
</code>
</pre>
    </td>
    <td>

<pre><code class="language-lua">loka funkcio estas_sekura(vico_posteno, kolumno_posteno, pozicioj)
  por antaŭa_vico = 1, vico_posteno - 1 fare
    loka antaŭa_kolumno = pozicioj[antaŭa_vico]
    se antaŭa_kolumno == kolumno_posteno tiam
      reŝalte falsa
    hop
    se matematiko.abs(antaŭa_kolumno - kolumno_posteno) == (vico_posteno - antaŭa_vico) tiam
      reŝalte falsa
    hop
  hop
  reŝalte vera
hop

loka funkcio metu_reĝinojn(vico_posteno, tabulo_grando, pozicioj)
  se vico_posteno > tabulo_grando tiam
    reŝalte vera
  hop
  por kolumno_posteno = 1, tabulo_grando fare
    se estas_sekura(vico_posteno, kolumno_posteno, pozicioj) tiam
      pozicioj[vico_posteno] = kolumno_posteno
      se metu_reĝinojn(vico_posteno + 1, tabulo_grando, pozicioj) tiam
        reŝalte vera
      hop
    hop
  hop
  reŝalte falsa
hop</code></pre>
    </td>
  </tr>
</table>

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
