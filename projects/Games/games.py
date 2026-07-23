### My first programming project written in 2023!

import random

print("Welcome to Estevan's Games!")
playername = input("Enter name: ")
print()
print(f'Welcome, {playername}!')
print()
while True:
    print("Select a game from the following options:")
    print("1: Paper, Rock, Scissors!")
    print("2: Battle Card Game")
    print("0: or Quit program")
    print()

    game_selection = int(input("Game selection: "))
    print()
    if game_selection == 0:
        break
    elif game_selection == 1:
#1-PAPER, ROCK, SCISSORS!
        print("Welcome to Paper, Rock, Scissors!")
        print("Paper beats Rock")
        print("Rock beats Scissors")
        print("Scissors beats Paper")
        print()

        win = 0
        lose = 0
        draw = 0

        while True:
            player = int(input("1 - Paper, 2 - Rock, 3 - Scissors, or 0 - Quit?: "))
            if player == 0:
                break
            elif player == 1:
                print("You selected: Paper")
            elif player == 2:
                print("You selected: Rock")
            elif player == 3:
                print("You selected: Scissors")
            else:
                player > 4
                print("Invalid selection, please try again.")
                continue

            computer = random.randint(1,3)
            if computer == 1:
                print("Computer selected: Paper")
            elif computer == 2:
                print("Computer selected: Rock")
            else:
                computer == 3
                print("Computer selected: Scissors")

            #1-paper 2-rock 3-scissors
            if (player == 1 and computer == 2) or (player == 2 and computer == 3) or (player ==3 and computer ==1):
                print("You won!")
                print()
                win += 1
            elif (player == 2 and computer == 1) or (player == 3 and computer == 2) or (player == 1 and computer == 3):
                print("You lost!")
                print()
                lose += 1
            else:
                (computer == player)
                print("DRAW!")
                print()
                draw += 1


        print()
        print("Wins:     ", win)
        print("Losses:   ", lose)
        print("Draws:    ", draw)
        print()
        if win < lose:
            print("Better luck next time against the computer, kiddo! :(")
        elif lose < win:
            print("Rise against the machine! :)")
            prs = open("PRS high score.txt.","a")
            prs.write(f"{playername}'s High Score: {win}\n")
            prs.close()
        else:
            draw = win + lose
            print("No winners today :(")
        print()
#END OF PAPER, ROCK, SCISSORS
            
    elif game_selection == 2:
#2-BATTLE CARD GAME
        #START HERE#
        print("Welcome to Battle Card Game!")
        print("Players have 13 cards.")
        print("Player with the higher card value wins. Suits do not take place!")

        play = 'y'
        while play == 'y' or 'yes':
            cardNamesPlayer = ['two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'jack', 'queen', 'king', 'ace']
            cardValuesPlayer = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
            cardNamesComputer = ['two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'jack', 'queen', 'king', 'ace']
            cardValuesComputer = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]

            pscore = 0
            cscore = 0
            hold = input("Press Enter to continue")
            print()

            for i in range(13):
                player = random.randint(0, len(cardValuesPlayer) - 1)
                computer = random.randint(0, len(cardValuesComputer) - 1)
                print("Round", i + 1)
                print("-" * 7)
                print(f"{playername}:        ", cardNamesPlayer[player])
                print("Computer's card:  ", cardNamesComputer[computer])
                if cardValuesPlayer[player] > cardValuesComputer[computer]:
                    pscore += 1
                    print("You win! :)")
                    print()
                elif cardValuesPlayer[player] < cardValuesComputer[computer]:
                    cscore += 1
                    print("Computer wins! :(")
                    print()
                else:
                    cardValuesPlayer[player] == cardValuesComputer[computer]
                    print("Tie! :/")
                    print()
                cardNamesPlayer.pop(player)
                cardValuesPlayer.pop(player)
                cardNamesComputer.pop(computer)
                cardValuesComputer.pop(computer)
                print("Score:")
                print(f"{playername}", pscore, " | ""Computer:", cscore)


                print()
                hold = input("Press Enter to continue")
                print()
            if pscore > cscore:
                print("YOU'RE THE WINNER")
                bc = open("BC high score.txt.","a")
                bc.write(f"{playername}'s High Score: {pscore}\n")
                bc.close()
            elif pscore < cscore:
                print("You lost :(")
            else:
                pscore == cscore
                print("No winners or losers today :/")
            print()
            print(f'{playername}:       ', pscore)
            print("Computer's score: ", cscore)
            print()
            print("Play again? 'y' for yes")
            print("Otherwise press any character to continue..")
            play = input().lower()
            if play != 'y':
                break
        print(f"Thanks for playing, {playername}")
        print()
#END OF BATTLE CARD GAME

    elif game_selection > 2:
        print("Invalid selection. Please try again.")
    
print(f"See you later, {playername}")
close_program = input("Press return to exit..")