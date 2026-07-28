# Lokaj agentaj instrukcioj por Lupa

## Lingvo de publikigata enhavo

Ĉiam publikigu en Esperanto:

- mesaĝojn de enmetadoj (komitoj);
- dokumentaron;
- Lupa-testojn, inkluzive testajn nomojn, komentojn kaj erarmesaĝojn;
- numerojn;
- MR-titolojn, priskribojn kaj komentojn;
- komentojn destinitajn por publikigo.

## Lingvo de `fontaro/`

Ĝis eksplicita alia instrukcio, konservu en la angla ĉiujn novajn kaj ŝanĝatajn kodajn elementojn en `fontaro/`: identigilojn, internajn nomojn, komentojn kaj uzantvideblajn teknikajn mesaĝojn. Tio estas provizora konvencio, planata por posta evoluo.

La jam ekzistanta escepto estas **LoTPoCIo/LTPC(I)**, vortluda traduko de REPL, laŭ la dokumentaro. Ne renomigu aŭ angligu ĝin sen eksplicita peto.

## Stilo de Lupa-testoj

- Preferu `ja` aŭ `loke` anstataŭ la Lua-retrokongrua formo `local`, krom kiam la testo specife kontrolas tiun retrokongruan formon.
- Ne uzu unuliterajn identigilojn, krom kiam ili estas esencaj al la testcelo. Elektu laŭkuntekstan Esperantan vorton validan laŭ PIV.

## Konservindaj laboraj kutimoj

- Tenu ĉiun ŝanĝon malgranda kaj unucela; disigu neparencajn plibonigojn.
- Ĝisdatigu `dokumentaro/esperantaj-sinonimoj.md` kiam ŝanĝiĝas leksila aŭ biblioteka sinonimo: ĝi estas la fonto de vero.
- Aldonu regresan teston al `testaro/` por ĉiu nova sintakso, sinonimo aŭ riparita rando; kovru kaj diakritan kaj `-x` formojn kiam ambaŭ estas subtenataj.
- Por leksilaj kaj unikodaj ŝanĝoj, testu vortlimojn, apartigilojn, miksitan ASCII/Unikodon, malvalidajn formojn kaj kongruon kun kanona Lua-kodo.
- Klarigu rompoŝanĝojn en `protokolo.md`, ĝisdatigu la version kiam necese, kaj migru la ekzistantan testaron.
- Antaŭ MR, rulu `./fare plentesti`; en la MR-priskribo resumu la celon, ŝanĝojn, rilatajn numerojn kaj validigon.
- Konservu Lua-kongruon kaj la originalajn Lua-kreditojn; ne ŝanĝu ambaŭ sen eksplicita pravigo kaj teste.

## Agnoskoj kaj kunverkintaro

- **Neniam uzu la "Co-Authored-By" kampo en git-komitmesaĝoj por menci programarilojn (ekz. Mistral Vibe, Copilot).** Aŭtorrajto estas rezervita por homaj kreintoj, kaj neniu jurisdikcio agnoskas aŭtorrajtojn por iloj.
- Se programarilo mankas en la kreditoj de `README.md` (sub "Agnoskoj > Aliaj iloj"), vi povas aldoni ĝin tie.
