# L17 - HW/SW-integrationstestning samt säker och robust mjukvara

## Dagordning
* HW/SW-integrationstestning: att verifiera det färdiga systemet mot fysisk hårdvara.
* Vad går att automatisera (t.ex. via seriell kommunikation), och vad kräver manuell
  verifiering?
* Demo: en self-hosted runner med ett inkopplat kort, som flashar firmware-artefakten från
  pipelinen och kör test mot fysisk hårdvara (hardware-in-the-loop).
* Säker och robust mjukvara: defensiv programmering, hantering av ogiltig indata och
  gränsfall i produktionskod.
* Fortsatt arbete med **P04** (VG).

---

## Mål med lektionen
* Kunna avgöra vilka integrationstester som rimligen kan automatiseras, och vilka som kräver
  manuell verifiering.
* Kunna dokumentera ett manuellt genomfört integrationstest på ett spårbart sätt.
* Kunna identifiera vanliga orsaker till att ett inbyggt system blir mindre robust, och föreslå lämpliga åtgärder.
* Kunna redogöra för vad en self-hosted runner möjliggör, och vilka praktiska baksidor den för
  med sig.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_integration_and_robustness.md) om HW/SW-integrationstestning och
  robust mjukvara.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.

---

## Utvärdering
* Vilket integrationstest genomförde ni, och varför gick det (eller gick det inte) att
  automatisera?
* Vilka delar av er kodbas är mest känsliga för ogiltig indata, och hur hanteras det i dag?
* Vad hade krävts för att flytta just ert manuella testfall in i pipelinen, och hade det varit
  värt besväret?

---

## Nästa lektion
* Projektarbete (del I).

---
