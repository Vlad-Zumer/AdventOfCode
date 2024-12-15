// TY DAY[9] -> https://www.youtube.com/watch?v=BXY6OZsA4TA
using System.Text;

namespace AOT.Day9;

// dotnet run --property:Configuration=Release
static class Program
{
    struct ParsedInputItem
    {
        public Int32 id;
        public Byte size;

        public override string ToString()
        {
            return $"({id},{size})";
        }
    }

    struct ReplacePartitions
    {
        public ParsedInputItem filledDest;
        public ParsedInputItem filledSrc;
        public ParsedInputItem? remainingDest;
        public ParsedInputItem? remainingSrc;
    }

    static ParsedInputItem[] ParseInput(string fileName)
    {
        List<ParsedInputItem> results = [];
        string[] lines = File.ReadAllLines(fileName);
        if (lines.Length > 1)
            throw new Exception("Input expected to have only 1 line");
        string line = lines[0];

        bool isSpace = false;
        Int32 id = 0;
        foreach (char ch in line)
        {
            Int32 nid;
            if (isSpace)
            {
                nid = -1;
            }
            else
            {
                nid = id;
                id++;
            }

            results.Add(new ParsedInputItem { id = nid, size = (byte)(ch - '0') });
            isSpace = !isSpace;
        }

        return results.ToArray();
    }

    static bool IsFreeSpace(ParsedInputItem item)
    {
        return item.id == -1;
    }

    static ReplacePartitions GetReplacePartition(in ParsedInputItem dest, in ParsedInputItem source)
    {
        ReplacePartitions ret = new ReplacePartitions { filledDest = dest, filledSrc = source, remainingDest = null, remainingSrc = null };
        if (source.size == dest.size)
        {
            ret.filledDest = source;
            ret.filledSrc = dest;
            return ret;
        }
        else if (source.size < dest.size)
        {
            ret.filledDest = source;
            ret.filledSrc = new ParsedInputItem { id = dest.id, size = source.size };
            ret.remainingDest = new ParsedInputItem { id = dest.id, size = (byte)(dest.size - source.size) };
            return ret;
        }
        else
        {
            // src.size > dest.size
            ret.filledDest = new ParsedInputItem { id = source.id, size = dest.size };
            ret.filledSrc = dest;
            ret.remainingSrc = new ParsedInputItem { id = source.id, size = (byte)(source.size - dest.size) };
            return ret;
        }
    }

    static ulong Checksum(in ParsedInputItem[] input)
    {
        ulong sum = 0;
        ulong index = 0;
        foreach (var item in input)
        {
            if (IsFreeSpace(item))
            {
                index += item.size;
                continue;
            }

            ulong localId = (ulong)item.id;
            byte localSize = item.size;
            while (localSize > 0)
            {
                sum += index * localId;
                index++;
                localSize--;
            }
        }

        return sum;
    }

    static ulong P1(in ParsedInputItem[] input)
    {
        List<ParsedInputItem> local = ((ParsedInputItem[])input.Clone()).ToList();
        int indexL = 0;
        int indexR = local.Count - 1;

        while (indexL < indexR)
        {
            if (!IsFreeSpace(local[indexL]))
            {
                indexL++;
                continue;
            }

            if (IsFreeSpace(local[indexR]))
            {
                indexR--;
                continue;
            }

            ReplacePartitions partitions = GetReplacePartition(local[indexL], local[indexR]);
            local[indexL] = partitions.filledDest;
            local[indexR] = partitions.filledSrc;
            if (partitions.remainingDest.HasValue)
            {
                local.Insert(indexL + 1, partitions.remainingDest.Value);
                indexR++;
            }

            if (partitions.remainingSrc.HasValue)
            {
                local.Insert(indexR, partitions.remainingSrc.Value);
                indexR++;
            }

        }
        return Checksum(local.ToArray());
    }

    static ulong P2(in ParsedInputItem[] input)
    {
        List<ParsedInputItem> local = ((ParsedInputItem[])input.Clone()).ToList();

        int index = local.Count - 1;
        while (index >= 0)
        {
            if (IsFreeSpace(local[index]))
            {
                index--;
                continue;
            }

            ParsedInputItem curr = local[index];
            int foundIndex = local.FindIndex(item => IsFreeSpace(item) && item.size >= curr.size);
            if (foundIndex == -1 || foundIndex >= index)
            {
                index--;
                continue;
            }

            ReplacePartitions partitions = GetReplacePartition(local[foundIndex], local[index]);
            local[foundIndex] = partitions.filledDest;
            local[index] = partitions.filledSrc;
            if (partitions.remainingDest.HasValue)
            {
                local.Insert(foundIndex + 1, partitions.remainingDest.Value);
                index++;
            }
        }

        return Checksum(local.ToArray());
    }


    static void Main(string[] args)
    {
        // string fileName = "test.txt";
        string fileName = "input.txt";
        var input = ParseInput(fileName);

        Console.WriteLine("P1: {0}", P1(input));
        Console.WriteLine("P2: {0}", P2(input));
    }
}
