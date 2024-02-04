#!/bin/zsh

function read_results {
    local log_file="$(dirname "$0")/log.txt"
    if [ -f "$log_file" ]; then
        echo "Game history:"
        cat "$log_file"
    else
        echo "File $log_file does not exist"
    fi
}

function log_results {
    local result="$1"
    local log_file="$(dirname "$0")/log.txt"
    local timestamp=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[$timestamp] $result" >> "$log_file"
}

function shuffle_cards {
    local i j temp
    for ((i = 16 - 1; i > 0; i--)); do
        j=$((RANDOM % i + 1))
        temp=${cards[$i]}
        cards[$i]=${cards[$j]}
        cards[$j]=$temp
    done
}

board=(
    " " " " " " " "
    " " " " " " " "
    " " " " " " " "
    " " " " " " " "
)
cards=(
    "A" "A" "B" "B" 
    "C" "C" "D" "D"
    "E" "E" "F" "F"
    "G" "G" "H" "H"
)

player1_pairs=0
player2_pairs=0

function display_board {
    clear
    echo "+---+---+---+---+"
    echo "| ${board[1]} | ${board[2]} | ${board[3]} | ${board[4]} |"
    echo "+---+---+---+---+"
    echo "| ${board[5]} | ${board[6]} | ${board[7]} | ${board[8]} |"
    echo "+---+---+---+---+"
    echo "| ${board[9]} | ${board[10]} | ${board[11]} | ${board[12]} |"
    echo "+---+---+---+---+"
    echo "| ${board[13]} | ${board[14]} | ${board[15]} | ${board[16]} |"
    echo "+---+---+---+---+"
}

function check_winner {
    local matched_count=0
    for cell in "${board[@]}"; do
        if [[ "$cell" == " " ]]; then
            return
        fi
    done

    echo "The game is over! All the cards are open."

    if (( player1_pairs > player2_pairs )); then
        echo "Player_1 wins! $player1_pairs - $player2_pairs"
        log_results "Player_1 wins! $player1_pairs - $player2_pairs"
    elif (( player2_pairs > player1_pairs )); then
        echo "Player_2 wins! $player2_pairs - $player1_pairs"
        log_results "Player_2 wins! $player2_pairs - $player1_pairs"
    else
        echo "Draw! $player1_pairs - $player2_pairs"
        log_results "Draw! $player1_pairs - $player2_pairs"
    fi

    read_results

    exit 0

}

function main {
    local current_player=1
    local selected=()

    shuffle_cards

    while true; do
        display_board

        if (( ${#selected[@]} == 2 )); then
            if [[ "${cards[selected[1]]}" == "${cards[selected[2]]}" ]]; then
                echo "Player_$current_player found a matching pair!"
                if ((current_player == 1)); then
                    ((player1_pairs++))
                else
                    ((player2_pairs++))
                fi
            else
                board[selected[1]]=" "
                board[selected[2]]=" "
                echo "The cards didn't match."
                ((current_player == 1)) && current_player=2 || current_player=1
            fi
            selected=()
        fi

        check_winner
        echo "Points: Player_1 - $player1_pairs, Player_2 - $player2_pairs"
        echo "Enter index (1-16)"
        echo -n "Player_$current_player: "
        read index

        if [[ "$index" =~ ^[1-9]$ || "$index" =~ ^1[0-6]$ ]] && [[ "${board[index]}" == " " ]]; then
            selected+=($index)
            board[index]=${cards[index]}
        else
            echo "Invalid index or card has already been matched."
            sleep 2
        fi
    done
}


main