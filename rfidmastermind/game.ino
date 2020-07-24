byte gamePhase = 0;
byte targetColors[TURNS];
byte turnColors[ROUNDS][TURNS];
byte roundCorrect[ROUNDS];
byte turn = 0;
byte currentRound = 0;
uint32_t lastTurn = 0;

void GAME_Init()
{
    LEDS_Flash(strip.Color(55, 0, 55), 2, 60);
    currentRound = 0;
    turn = 0;
    lastTurn = 0;
    byte colors[TURNS] = {1, 2, 3, 4};

    for (int i = 0; i < TURNS; i++)
    {
        targetColors[i] = 0;
        roundCorrect[i] = 0;

        for (int o = 0; o < ROUNDS; o++)
        {
            turnColors[o][i] = 0;
        }
    }

    for (byte i = 0; i < TURNS; i++)
    {
        byte newIndex = floor(random(0, 4));

        if (colors[newIndex] != 0)
        {
            Serial.println(colors[newIndex]);
            targetColors[i] = colors[newIndex];
            colors[newIndex] = 0;
        }
        else
        {
            i--;
        }
    }

    gamePhase = 0;
}

void GAME_Update()
{
    switch (gamePhase)
    {
    case 0:
        GAME_Start_Turn();
        break;

    case 1:
        GAME_Entry_Update();
        break;

    case 2:
        GAME_End_Turn();
        break;
    }
}

void GAME_Start_Turn()
{
    Serial.print("Round ");
    Serial.print(currentRound);
    Serial.print(", Turn");
    Serial.print(turn);
    Serial.println(" Start");
    gamePhase = 1;
}

void GAME_Entry_Update()
{
    uint32_t cardid = RFID_Read();

    if (cardid != NULL && cardid != lastTurn)
    {
        lastTurn = cardid;
        
        if (cardid == RED_CARD_ID)
        {
            LEDS_SetAll(RED_DIM_COLOR);
            turnColors[currentRound][turn] = 1;
            Serial.println("red ");
        }

        if (cardid == GREEN_CARD_ID)
        {
            LEDS_SetAll(GREEN_DIM_COLOR);
            turnColors[currentRound][turn] = 2;
            Serial.println("green ");
        }

        if (cardid == BLUE_CARD_ID)
        {
            LEDS_SetAll(BLUE_DIM_COLOR);
            turnColors[currentRound][turn] = 3;
            Serial.println("blue ");
        }

        if (cardid == YELLOW_CARD_ID)
        {
            LEDS_SetAll(YELLOW_DIM_COLOR);
            turnColors[currentRound][turn] = 4;
            Serial.println("yellow ");
        }

        delay(300);
        LEDS_SetAll(strip.Color(0, 0, 0));
        gamePhase = 2;
    }
}

void GAME_End_Turn()
{
    LEDS_SetAll(strip.Color(0, 0, 0));
    GAME_Draw_Turns();
    strip.show();
    delay(1000);
    gamePhase = 0;
    turn++;

    if (turn >= TURNS)
    {
        GAME_End_Round();
    }
}

void GAME_End_Round()
{
    lastTurn = 0;
    Serial.println("round over");
    int correct = 0;

    for (byte i = 0; i < TURNS; i++)
    {
        if (turnColors[currentRound][i] == targetColors[i])
        {
            correct++;
            LEDS_SetAll(strip.Color(55, 55, 55));
            delay(220);
            LEDS_SetAll(strip.Color(0, 0, 0));
            delay(220);
        }
    }

    Serial.print("Correct ");
    Serial.println(correct);
    roundCorrect[currentRound] = correct;

    if (correct == TURNS)
    {
        LEDS_Flash(strip.Color(0, 55, 0), 5, 100);
        GAME_Draw_Turns();
        delay(5000);
        appPhase = 0;
        Serial.println("You won!");
    }
    else
    {
        if (correct == 0)
        {
            GAME_Draw_Turns();
            delay(100);
            LEDS_SetAll(strip.Color(0, 0, 0));
            delay(100);
            GAME_Draw_Turns();
            delay(100);
            LEDS_SetAll(strip.Color(0, 0, 0));
            delay(100);
        }

        GAME_Draw_Turns();
        turn = 0;
        currentRound++;
    }

    if (currentRound == ROUNDS)
    {
        LEDS_Flash(strip.Color(55, 0, 0), 5, 100);
        GAME_Draw_Turns();
        delay(5000);
        appPhase = 0;
        Serial.println("You lost!");
    }
}

void GAME_Draw_Turns()
{ 
    for (byte o = 0; o < ROUNDS; o++)
    {
        byte correctThisRound = roundCorrect[o];

        for (byte i = 0; i < TURNS; i++)
        {
            switch (turnColors[o][i])
            {
            case 1:
                if (correctThisRound > 0)
                    strip.setPixelColor(o + (i * 5), RED_COLOR);
                else
                    strip.setPixelColor(o + (i * 5), RED_DIM_COLOR);
                break;

            case 2:
                if (correctThisRound > 0)
                    strip.setPixelColor(o + (i * 5), GREEN_COLOR);
                else
                    strip.setPixelColor(o + (i * 5), GREEN_DIM_COLOR);
                break;

            case 3:
                if (correctThisRound > 0)
                    strip.setPixelColor(o + (i * 5), BLUE_COLOR);
                else
                    strip.setPixelColor(o + (i * 5), BLUE_DIM_COLOR);
                break;

            case 4:
                if (correctThisRound > 0)
                    strip.setPixelColor(o + (i * 5), YELLOW_COLOR);
                else
                    strip.setPixelColor(o + (i * 5), YELLOW_DIM_COLOR);
                break;
            }

            if (correctThisRound > 0)
                correctThisRound--;
        }
    }
    strip.show();
}
