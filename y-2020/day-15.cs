using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;

class App
{
    public Dictionary<long, Tuple<long, long>> NumberAndLast2Turns = new Dictionary<long, Tuple<long, long>>();
    public long CurrentTurn = 1;
    public long LastNumber = -1;

    public IEnumerable<long> InputData;

    public App(IEnumerable<long> inputData)
    {
        InputData = inputData;
        InitTurns();
    }

    public void InitTurns()
    {
        foreach (long item in InputData)
        {
            AddNumberToDic(item);
            CurrentTurn++;
        }
    }

    public void AddNumberToDic(long number)
    {
        if (NumberAndLast2Turns.ContainsKey(number))
        {
            Tuple<long, long> oldVal = NumberAndLast2Turns[number];
            NumberAndLast2Turns[number] = new Tuple<long, long>(CurrentTurn, oldVal.Item1);
        }
        else
        {
            NumberAndLast2Turns.Add(number, new Tuple<long, long>(CurrentTurn, -1));
        }

        LastNumber = number;
    }

    public long TakeTurn()
    {
        Tuple<long, long> turnsNumberWasSaid = NumberAndLast2Turns[LastNumber];
        if (turnsNumberWasSaid.Item2 == -1)
        {
            return 0;
        }
        else
        {
            return (turnsNumberWasSaid.Item1 - turnsNumberWasSaid.Item2);
        }
    }

    public void printDic()
    {
        IEnumerable<string> dicStr = NumberAndLast2Turns.Select(kvp => $"({kvp.Key}, ({kvp.Value.Item1} , {kvp.Value.Item2}))");
        Console.WriteLine($"CT:{CurrentTurn} [ {string.Join(", ", dicStr)} ]");
    }

    public void Solve1ForTurn(long turn)
    {
        long saidNumber = 0;
        while(CurrentTurn <= turn)
        {
            saidNumber = TakeTurn();
            // printDic();
            AddNumberToDic(saidNumber);
            CurrentTurn++;
            // if (CurrentTurn % 100000L == 0)
            // {
            //     printDic();
            // }
        }

        Console.WriteLine(saidNumber);
    }

    public void Reset()
    {
        CurrentTurn = 1;
        LastNumber = -1;
        NumberAndLast2Turns.Clear();
        InitTurns();
    }

    public static void Main(string[] args)
    {
        App sols = new App(new List<long> {2,1,10,11,0,6});
        sols.Solve1ForTurn(2020);
        sols.Reset();
        sols.Solve1ForTurn(30000000L);
    }
}

App.Main(new string[0]); // --Run with scriptscs