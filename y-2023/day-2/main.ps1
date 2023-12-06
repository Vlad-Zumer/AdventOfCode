###############################
##   REQUIRES PS >= 5
###############################

if ($PSVersionTable.PSVersion.Major -lt 5) {
    Write-Host -ForegroundColor Red "Requires at least version Powershell version 5 to run"
    exit 1
}

# $inputLines = Get-Content .\test.input
$inputLines = Get-Content .\input.txt

$p1Max = @{
    Red = 12;
    Green = 13;
    Blue = 14;
}

function IsPossibleGame($game) {
    return $p1Max.Red -ge $game.MaxCubesSeen.Red -and
           $p1Max.Green -ge $game.MaxCubesSeen.Green -and
           $p1Max.Blue -ge $game.MaxCubesSeen.Blue;
}

function GamePower($game) {
    return $game.MaxCubesSeen.Red * $game.MaxCubesSeen.Green * $game.MaxCubesSeen.Blue
}

function PrintGame($game) {
    Write-Host "Game:"
    Write-Host "  Number: $($game.Number)"
    Write-Host "  MaxCubesSeen:"
    Write-Host "    Red:$($game.MaxCubesSeen.Red)"
    Write-Host "    Green:$($game.MaxCubesSeen.Green)"
    Write-Host "    Blue:$($game.MaxCubesSeen.Blue)"
}

$p1Sum=0;
$p2Sum=0;

foreach ($line in $inputLines) 
{
    # Write-Host "Line: $line"
    $gameSplit = $line -isplit ':'
    $gameNumber = [int]($gameSplit[0] -isplit ' ')[1]

    $game = @{
        Number = $gameNumber;
        MaxCubesSeen = @{
            Red = 0;
            Green = 0;
            Blue = 0;
        }
    }

    # Write-Host "  Game Number: $gameNumber"

    $gameRounds = $gameSplit[1] -isplit ';';

    # Write-Host "  Game Rounds:"
    foreach ($gameRound in $gameRounds) {
        # Write-Host "    Round: '$($gameRound.trim())'"
        $cubesInRound = $gameRound -isplit ','
        foreach ($cubes in $cubesInRound){
            $cubes = $cubes.trim()
            $cubesSplit = $cubes -isplit ' '
            $cubeNumber = [int]$cubesSplit[0]
            $cubeColor = $cubesSplit[1].ToLower()

            if ($cubeColor -eq "red" -and $cubeNumber -gt $game.MaxCubesSeen.Red) {
                $game.MaxCubesSeen.Red = $cubeNumber
            }
            if ($cubeColor -eq "green" -and $cubeNumber -gt $game.MaxCubesSeen.Green) {
                $game.MaxCubesSeen.Green = $cubeNumber
            }
            if ($cubeColor -eq "blue" -and $cubeNumber -gt $game.MaxCubesSeen.Blue) {
                $game.MaxCubesSeen.Blue = $cubeNumber
            }

            # Write-Host "      Cubes: {number: $cubeNumber, color: $cubeColor}"
        }
    }

    # PrintGame $game
    
    # Write-Host "Game: $($game.Number) - Power: $(GamePower $game)"
    $p2Sum = $p2Sum + $(GamePower $game)
    
    if (IsPossibleGame $game)
    {
        $p1Sum = $p1Sum + $game.Number
        # Write-Output "Game Possible $($game.Number)"
    }
    
    # Write-Host ""
}

Write-Host "P1: $p1Sum"
Write-Host "P2: $p2Sum"
