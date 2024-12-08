namespace AOT.Day5;

using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using OrderPair = (int before, int after);

static class Program
{
    static (OrderPair[] orderPairs, int[][] updates) ParseInput(string[] lines)
    {
        List<OrderPair> orderPairs = [];
        List<int[]> updates = [];
        IEnumerator<string> lineIt = lines.AsEnumerable().GetEnumerator();
        while (lineIt.MoveNext())
        {
            string line = lineIt.Current;
            if (line.Trim() == "")
            {
                break;
            }
            string[] parts = line.Split('|');
            orderPairs.Add((int.Parse(parts[0]), int.Parse(parts[1])));
        }

        while (lineIt.MoveNext())
        {
            string line = lineIt.Current;
            string[] parts = line.Split(',');
            updates.Add(parts.Select(el => int.Parse(el)).ToArray());
        }

        return (orderPairs.ToArray(), updates.ToArray());
    }

    static (ulong p1, ulong p2) Solve(OrderPair[] orderPairs, int[][] updates)
    {
        ulong sumP1 = 0;
        ulong sumP2 = 0;

        // expectedAfterMap[value] -> values that appear in the after field
        Dictionary<int, HashSet<int>> expectedAfterMap = orderPairs.GroupBy(e => e.before)
                                                            .Select(grouping => (grouping.Key, Items: grouping.Select(el => el.after)))
                                                            .ToDictionary(el => el.Key, el => new HashSet<int>(el.Items));

        // expectedBeforeMap[value] -> values that appear in the before field
        Dictionary<int, HashSet<int>> expectedBeforeMap = orderPairs.GroupBy(e => e.after)
                                                            .Select(grouping => (grouping.Key, Items: grouping.Select(el => el.before)))
                                                            .ToDictionary(el => el.Key, el => new HashSet<int>(el.Items));

        int checkUpdate(int[] update)
        {
            for (int curIndex = 0; curIndex < update.Length; curIndex++)
            {
                int value = update[curIndex];
                int[] before = (curIndex - 1 < 0) ? [] : update[0..(curIndex - 1)];
                int[] after = (curIndex + 1 >= update.Length) ? [] : update[(curIndex + 1)..^0];
                if ((expectedAfterMap.ContainsKey(value) && expectedAfterMap[value].Overlaps(before))
                    || (expectedBeforeMap.ContainsKey(value) && expectedBeforeMap[value].Overlaps(after)))
                {
                    return curIndex;
                }
            }
            return -1;
        }

        foreach (int[] update in updates)
        {
            int badIndex = checkUpdate(update);

            if (badIndex == -1)
            {
                // Console.WriteLine("[{0}]", string.Join(',' ,update.Select(el => el.ToString())));
                sumP1 += (ulong)update[update.Length / 2];
            }
            else
            {
                // fix update
                // remove issue and add it to the end
                //
                //
                // ####################################################
                // #                                                  #
                // #        this is NOT guaranteed to work            #
                // #                                                  #
                // ####################################################
                //
                //
                // this will fail if the problem was with the last element

                int[] fixedUpdate = (int[])update.Clone();
                List<int> aux;
                while (badIndex != -1)
                {
                    int value = fixedUpdate[badIndex];
                    aux = fixedUpdate.ToList();
                    aux.RemoveAt(badIndex);
                    aux.Add(value);
                    fixedUpdate = aux.ToArray();
                    badIndex = checkUpdate(fixedUpdate);
                }

                // Console.WriteLine("[{0}]", string.Join(',' ,update.Select(el => el.ToString())));
                sumP2 += (ulong)fixedUpdate[fixedUpdate.Length / 2];
            }
        }

        return (sumP1, sumP2);
    }

    static void Main(string[] args)
    {
        // string fileName = "test.txt";
        string fileName = "input.txt";
        string[] lines = File.ReadAllLines(fileName);
        (OrderPair[] orderPairs, int[][] updates) = ParseInput(lines);
        (ulong p1, ulong p2) = Solve(orderPairs, updates);
        Console.WriteLine("P1 = {0}", p1);
        Console.WriteLine("P2 = {0}", p2);
        Console.WriteLine("DONE");
    }
}
