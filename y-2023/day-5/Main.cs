using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Data;

public record InclRange(ulong min, ulong max);
public record ValueMapEntry(InclRange from, InclRange to);
public record ValueMapMapping(string from, string to);

public class DynamicComparer<T> : IComparer<T>
{
    private Comparison<T> comparison;

    public DynamicComparer(Comparison<T> comparison)
    {
        this.comparison = comparison;
    }

    public int Compare(T? x, T? y)
    {
        if (x == null || y == null)
        {
            throw new Exception("Cannot compare null");
        }
        return comparison(x, y);
    }
}

public static class Extensions
{
    public static void AddSorted<T>(this List<T> list, T value, Comparison<T> comparison)
    {

        int indexToInsert = list.BinarySearch(value, new DynamicComparer<T>(comparison));
        if (indexToInsert < 0)
        {
            indexToInsert = ~indexToInsert;
        }
        list.Insert(indexToInsert, value);
    }
}

class App
{
    static void ParseInput(
        IEnumerable<string> inputLines,
        out List<ulong> seeds,
        out Dictionary<ValueMapMapping, List<ValueMapEntry>> mapMappingToMap)
    {
        RegexOptions regexOptions = RegexOptions.Multiline | RegexOptions.IgnoreCase | RegexOptions.Compiled;
        Regex mapLineRegex = new(@"(?<from>\w+)-to-(?<to>\w+)\s+map:", regexOptions);
        Regex mapValuesLineRegex = new(@"^\s*(?<to>\d+)\s+(?<from>\d+)\s+(?<span>\d+)$", regexOptions);


        mapMappingToMap = new Dictionary<ValueMapMapping, List<ValueMapEntry>>();
        seeds = new List<ulong>();

        ValueMapMapping? currentMap = null;
        foreach (string line in inputLines.Select(l => l.Trim()))
        {
            if (line.StartsWith("seeds:"))
            {
                seeds = line.Split(':')[1]
                            .Split(' ')
                            .Where(str => str.Length != 0)
                            .Select(str => ulong.Parse(str))
                            .ToList();
            }
            else if (mapLineRegex.IsMatch(line))
            {
                Match match = mapLineRegex.Match(line);
                string from = match.Groups["from"].Value;
                string to = match.Groups["to"].Value;
                currentMap = new ValueMapMapping(from, to);
                mapMappingToMap.Add(currentMap, new List<ValueMapEntry>());
            }
            else if (line.Length > 0 && char.IsDigit(line[0]))
            {
                if (currentMap == null)
                {
                    Console.WriteLine($"Fucked up with line:'{line}'");
                    Environment.Exit(1);
                }

                Match match = mapValuesLineRegex.Match(line);
                string fromMatch = match.Groups["from"].Value;
                string toMatch = match.Groups["to"].Value;
                string spanMatch = match.Groups["span"].Value;

                ulong span = ulong.Parse(spanMatch);

                ulong fromMin = ulong.Parse(fromMatch);
                ulong fromMax = fromMin + span - 1;

                ulong toMin = ulong.Parse(toMatch);
                ulong toMax = toMin + span - 1;

                mapMappingToMap[currentMap].AddSorted(
                    new ValueMapEntry(new InclRange(fromMin, fromMax), new InclRange(toMin, toMax)),
                    (v1, v2) => v1.from.min.CompareTo(v2.from.min));
            }
        }
    }

    static ulong MapNumber(ulong number, List<ValueMapEntry> values)
    {
        int foundIndex = values.Select(val => val.from.min).ToList().BinarySearch(number);
        if (foundIndex < 0)
        {
            // ~(-1) = 0 => 0th element is bigger than number
            foundIndex = Math.Max((~foundIndex) - 1, 0);
        }
        ValueMapEntry possibleVal = values[foundIndex];

        if (possibleVal.from.min <= number && number <= possibleVal.from.max)
        {
            return number - possibleVal.from.min + possibleVal.to.min;
        }

        return number;
    }

    static List<(ulong original, ulong mapped)> MapNumbers(
        List<ulong> numbers,
        Dictionary<ValueMapMapping, List<ValueMapEntry>> mappings,
        string startMapName,
        string endMapName)
    {
        Dictionary<string, ValueMapMapping> fromStrToMapping = mappings.Keys.ToDictionary(mapping => mapping.from, mapping => mapping);

        string currentMapping = startMapName;

        List<(ulong original, ulong mapped)> numberMapping = numbers.Select(s => (s, s)).ToList();
        while (currentMapping != endMapName)
        {
            ValueMapMapping mappingType = fromStrToMapping[currentMapping];
            List<ValueMapEntry> map = mappings[mappingType];
            numberMapping = numberMapping.Select(v => (v.original, MapNumber(v.mapped, map))).ToList();
            currentMapping = mappingType.to;
        }

        return numberMapping;
    }

    static List<InclRange> MapRange(InclRange range, List<ValueMapEntry> values)
    {
        // search for element that could contain the start of the range
        int foundIndex = values.Select(val => val.from.min).ToList().BinarySearch(range.min);
        if (foundIndex < 0)
        {
            // ~(-1) = 0 => 0th element is bigger than number
            foundIndex = Math.Max((~foundIndex) - 1, 0);
        }

        List<InclRange> retList = new();
        InclRange? remainingRange = range;

        if (range.min < values[foundIndex].from.min)
        {
            // the first element's min < range.min
            // (---[-----
            retList.Add(new InclRange(range.min, values[foundIndex].from.min-1));
            remainingRange = new InclRange(values[foundIndex].from.min, remainingRange.max);
        }

        for (int i = foundIndex; i < values.Count; i++)
        {
            // check to see if range can be mapped to the element
            // el.from = []
            // range = ()
            // [---(--------------]--)
            // [-------(------)---]
            // [------------------]-----(-------)
            // [ <= (
            ValueMapEntry el = values[i];
            
            // Console.WriteLine($"    rem:{remainingRange}");
            // Console.WriteLine($"    el:{el}");

            if (el.from.min > remainingRange.max)
            {
                // nothing to map anymore -> range stays the same
                retList.Add(remainingRange);
                remainingRange = null;
                break;
            }

            if (el.from.max < remainingRange.min)
            {
                // el cannot map this range
                continue;
            }

            // el contains the start of the range

            // offset from the start of the el to the start of mapped section
            ulong startOffset = remainingRange.min - el.from.min;

            // offset from the end of the el to the end of the mapped section
            ulong endOffset = Math.Min(remainingRange.max, el.from.max) - el.from.max;

            var mappedRange = new InclRange(el.to.min + startOffset, el.to.max + endOffset);
            retList.Add(mappedRange);

            // Console.WriteLine($"  Mapped Range {new InclRange(remainingRange.min, el.from.max + endOffset)} to {mappedRange}");
            ulong nextRangeMin = el.from.max + endOffset + 1;

            if (nextRangeMin > remainingRange.max)
            {
                remainingRange = null;
                break;
            }

            remainingRange = new InclRange(nextRangeMin, remainingRange.max);
            // Console.WriteLine($"  Remainder: {remainingRange}");
        }

        if (remainingRange != null)
        {
            retList.Add(remainingRange);
        }

        return retList;
    }

    static List<(InclRange original, InclRange mapped)> MapRanges(
        List<InclRange> ranges,
        Dictionary<ValueMapMapping, List<ValueMapEntry>> mappings,
        string startMapName,
        string endMapName)
    {
        Dictionary<string, ValueMapMapping> fromStrToMapping = mappings.Keys.ToDictionary(mapping => mapping.from, mapping => mapping);

        string currentMapping = startMapName;

        List<(InclRange original, InclRange mapped)> rangeMapping = ranges.Select(s => (s, s)).ToList();
        while (currentMapping != endMapName)
        {
            ValueMapMapping mappingType = fromStrToMapping[currentMapping];
            // Console.WriteLine(mappingType);
            List<ValueMapEntry> map = mappings[mappingType];
            rangeMapping = rangeMapping.SelectMany(v => MapRange(v.mapped, map).Select(r => (v.original, r))).ToList();
            currentMapping = mappingType.to;
            // foreach (var item in rangeMapping)
            // {
            //     Console.WriteLine($"  Result {item}");
            // }
        }

        return rangeMapping;
    }

    static void Main(string[] args)
    {
        // IEnumerable<string> lines = File.ReadLines("./test.txt");
        IEnumerable<string> lines = File.ReadLines("./input.txt");

        List<ulong> seeds;
        Dictionary<ValueMapMapping, List<ValueMapEntry>> mapMappingToMap;
        ParseInput(lines, out seeds, out mapMappingToMap);

        List<(ulong seed, ulong mapped)> seedToMapping = MapNumbers(seeds, mapMappingToMap, "seed", "location");
        (ulong seed, ulong location) p1 = seedToMapping.OrderBy(mapping => mapping.mapped).First();
        Console.WriteLine($"P1: {p1.location}");

        List<InclRange> seedRanges = new();
        for (int i = 0; i < seeds.Count; i += 2)
        {
            ulong span = seeds[i + 1];
            seedRanges.Add(new InclRange(seeds[i], seeds[i] + span - 1));
        }

        var p2 = MapRanges(seedRanges, mapMappingToMap, "seed", "location").OrderBy(o => o.mapped.min).First();
        Console.WriteLine($"P2: {p2.mapped.min}");
    }
}
