void RFID_Init()
{
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();

    if (!versiondata)
    {
        LEDS_SetAll(strip.Color(255, 0, 0));
        while (1)
            ;
    }

    nfc.SAMConfig();
}

uint32_t RFID_Read(void)
{
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t uidLength;

    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success)
    {
        nfc.PrintHex(uid, uidLength);

        if (uidLength == 4)
        {
            Serial.println("Len = 4");
            uint32_t cardid = uid[0];
            cardid <<= 8;
            cardid |= uid[1];
            cardid <<= 8;
            cardid |= uid[2];
            cardid <<= 8;
            cardid |= uid[3];
            Serial.println(cardid);

            return cardid;
        }

        if (uidLength == 7)
        {
            Serial.println("Len = 7, skipping first 3");
            uint32_t cardid = uid[3];
            cardid <<= 8;
            cardid |= uid[4];
            cardid <<= 8;
            cardid |= uid[5];
            cardid <<= 8;
            cardid |= uid[6];
            Serial.println(cardid);

            return cardid;
        }
    }

    return NULL;
}
