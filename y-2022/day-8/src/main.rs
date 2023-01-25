#![allow(non_snake_case)]
use std::{fs::File, io::{BufReader, BufRead}, ops::{Index, IndexMut}, fmt::Display, cmp::{min}, collections::HashSet, slice::Iter};

trait Stringify
{
    fn toString(&self) -> String;
}

trait MatrixWrap<ValType> {
    fn wrap(&self) -> SparseMatrixWrapper<ValType>;
}

trait MatrixWrapMut<ValType> {
    fn wrapMut(&mut self) -> MutSparseMatrixWrapper<ValType>;
}

trait SparseMatrix
{
    type MatrixValueType;

    fn getNumOfLines(&self) -> usize;
    fn getNumOfElInLine(&self, line: usize) -> usize;
    fn getLine(&self, line: usize) -> &Vec<Self::MatrixValueType>;
    fn getLineMut(&mut self, line: usize) -> &mut Vec<Self::MatrixValueType>;
    fn get (&self, line: usize, col: usize) -> Option<&Self::MatrixValueType>;
}

impl<ValType, T> MatrixWrap<ValType> for T
where T: SparseMatrix<MatrixValueType = ValType>
{
    fn wrap(&self) -> SparseMatrixWrapper<ValType>
    {
        return SparseMatrixWrapper {this: self};
    }
}

impl<ValType, T> MatrixWrapMut<ValType> for T
where T: SparseMatrix<MatrixValueType = ValType>
{
    fn wrapMut(&mut self) -> MutSparseMatrixWrapper<ValType>
    {
        return MutSparseMatrixWrapper {this: self};
    }
}

struct SparseMatrixWrapper<'a, ValueType>
{
    this: &'a dyn SparseMatrix<MatrixValueType = ValueType>
}

struct MutSparseMatrixWrapper<'a, ValueType>
{
    this: &'a mut dyn SparseMatrix<MatrixValueType = ValueType>
}

impl<T> Stringify for T
where T: SparseMatrix,
    T::MatrixValueType: Display
{
    fn toString(&self) -> String {
        let mut maxColWidths = Vec::new();
        
        for line in 0..self.getNumOfLines()
        {
            for col in 0..self.getNumOfElInLine(line)
            {
                let val = &self.wrap()[line][col];

                if maxColWidths.len() < col + 1
                {
                    maxColWidths.push(0);
                }

                if maxColWidths[col] < val.to_string().len()
                {
                    maxColWidths[col] = val.to_string().len();
                }
            }
        }
        
        let mut ret = String::new();

        for line in 0..self.getNumOfLines()
        {
            ret.push_str("| ");

            let COLS = self.getNumOfElInLine(line);
            for col in 0..COLS
            {
                let val = &self.wrap()[line][col]; 
                let valStr = val.to_string();
                ret.push_str(&valStr);
                
                if col + 1 < COLS
                {
                    ret.push_str(", ");
                }

                let spacesSize = maxColWidths[col] - valStr.len(); 
                let spaces = " ".repeat(spacesSize);
                ret.push_str(&spaces);
            }

            ret.push_str(" |\n");
        }

        ret.pop();

        return ret;
    }
}

impl<'a, T> Index<usize> for SparseMatrixWrapper<'a, T>
{
    type Output = Vec<T>;

    fn index(&self, index: usize) -> &Self::Output {
        return self.this.getLine(index);
    }
}

impl<'a, T> Index<usize> for MutSparseMatrixWrapper<'a, T>
{
    type Output = Vec<T>;

    fn index(&self, index: usize) -> &Self::Output {
        return self.this.getLine(index);
    }
}

impl<'a, T> IndexMut<usize> for MutSparseMatrixWrapper<'a, T>
{
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        return self.this.getLineMut(index);
    }
}

 trait Matrix : SparseMatrix 
{
    fn getNumOfCols(&self) -> usize;
}

#[derive(Debug, Clone, Copy)]
 enum MatrixIteratorType
{
    ColIterator,
    LineIterator,
    // Number of diags = Lines + Cols - 1
    // Number of el on diag = min(min(L,C), min(D+1, L+C-D-1)), where D = diag index
    PrimaryParallelDiagsIterator,
    SecondaryParallelDiagsIterator,
}

#[derive(Clone)]
 struct MatrixIterator<'a, T>
{
    matrix: &'a dyn Matrix<MatrixValueType = T>,
    itType: MatrixIteratorType,
    leftPointer: usize,
    rightOffset: usize,
    size: usize,
}

impl<'a, T> MatrixIterator<'a, T>
{
     fn new(matrix: &'a dyn Matrix<MatrixValueType = T>, iteratorType: MatrixIteratorType) -> Self
    {
        let size = MatrixIterator::calculateIteratorSize(&iteratorType, matrix);
        return MatrixIterator {
            itType: iteratorType,
            matrix,
            leftPointer: 0,
            rightOffset: 0,
            size,
        };
    }

    fn calculateIteratorSize(itType: &MatrixIteratorType, matrix: &'a dyn Matrix<MatrixValueType = T>) -> usize
    {
        return match itType {
            MatrixIteratorType::ColIterator => matrix.getNumOfCols(),
            MatrixIteratorType::LineIterator => matrix.getNumOfLines(),
            MatrixIteratorType::PrimaryParallelDiagsIterator => if matrix.getNumOfLines() > 0 { matrix.getNumOfLines() + matrix.getNumOfCols() - 1 } else { 0 },
            MatrixIteratorType::SecondaryParallelDiagsIterator => if matrix.getNumOfLines() > 0 { matrix.getNumOfLines() + matrix.getNumOfCols() - 1 } else { 0 },
        };
    }
}

impl <'a,T> Iterator for MatrixIterator<'a,T>
{
    type Item = MatrixComponentIterator<'a,T>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.size < self.rightOffset + self.leftPointer + 1
        {
            return None;
        }

        let ret = MatrixComponentIterator::new(self.matrix, self.leftPointer, self.itType);
        self.leftPointer += 1;

        return Some(ret);
    }

    fn size_hint(&self) -> (usize, Option<usize>) {
        let left = self.size - self.rightOffset - self.leftPointer;
        return (left, Some(left));
    }
}

impl <'a,T> DoubleEndedIterator for MatrixIterator<'a,T>
{
    fn next_back(&mut self) -> Option<Self::Item> {
        if self.size < self.rightOffset + self.leftPointer + 1
        {
            return None;
        }

        let ret = MatrixComponentIterator::new(self.matrix, self.size - self.rightOffset - 1, self.itType);
        self.rightOffset += 1;

        return Some(ret);
    }
}

impl <'a,T> ExactSizeIterator for MatrixIterator<'a,T>
{}

#[derive(Clone)]
 struct  MatrixComponentIterator<'a, T>
{
    matrix: &'a dyn Matrix<MatrixValueType = T>,
    outerItType: MatrixIteratorType,
    outerItPos: usize,
    leftPointer: usize,
    rightOffset: usize,
    size: usize,
}

impl<'a, T> MatrixComponentIterator<'a, T>
{
    fn new(matrix: &'a dyn Matrix<MatrixValueType = T>, outerItPos: usize, outerItType: MatrixIteratorType) -> Self
    {
        let size = MatrixComponentIterator::calculateIteratorSize(&outerItType, outerItPos, matrix);
        return MatrixComponentIterator {
            matrix,
            outerItType,
            outerItPos,
            leftPointer: 0,
            rightOffset: 0,
            size,
        };
    }

    fn calculateIteratorSize(outerItType: &MatrixIteratorType, outerItPos: usize,  matrix: &'a dyn Matrix<MatrixValueType = T>) -> usize
    {
        let lines = matrix.getNumOfLines();
        let cols = matrix.getNumOfCols();
        return match outerItType {
            MatrixIteratorType::ColIterator => matrix.getNumOfLines(),
            MatrixIteratorType::LineIterator => matrix.getNumOfCols(),
            MatrixIteratorType::PrimaryParallelDiagsIterator =>  min(min(lines,cols), min(outerItPos+1, lines+cols-outerItPos-1)),
            MatrixIteratorType::SecondaryParallelDiagsIterator =>  min(min(lines,cols), min(outerItPos+1, lines+cols-outerItPos-1)),
        };
    }

    fn getLineAndCol(&self, newItPos: usize) -> Option<(usize, usize)>
    {

        let lines = self.matrix.getNumOfLines();
        let cols = self.matrix.getNumOfCols();
        return match self.outerItType {
            MatrixIteratorType::ColIterator => Some((newItPos, self.outerItPos)),
            MatrixIteratorType::LineIterator => Some((self.outerItPos, newItPos)),
            MatrixIteratorType::PrimaryParallelDiagsIterator =>
            {
                // max (L - 1 - D, 0)
                let startLine = lines.checked_sub(1).unwrap_or(0).checked_sub(self.outerItPos).unwrap_or(0);
                
                // max (-(L-1-D), 0) = max(D+1-L)
                let startCol = (self.outerItPos + 1).checked_sub(lines).unwrap_or(0);

                Some((startLine+newItPos, startCol+newItPos))
            },
            MatrixIteratorType::SecondaryParallelDiagsIterator =>
            {
                // max (D - (C - 1), 0) = max (D + 1 - C, 0)
                let startLine = (self.outerItPos + 1).checked_sub(cols).unwrap_or(0);
                
                // min (C-1, D)
                let startCol = min(cols - 1, self.outerItPos);

                Some((startLine+newItPos, startCol-newItPos))
            },
        };
    }

}

impl <'a,T> Iterator for MatrixComponentIterator<'a,T>
{
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> 
    {
        if self.size < self.rightOffset + self.leftPointer + 1
        {
            return None;
        }

        let (line, col) = self.getLineAndCol(self.leftPointer)?;
        let ret = self.matrix.get(line, col);

        if ret.is_some()
        {
            self.leftPointer += 1;
        }

        return ret;
    }

    fn size_hint(&self) -> (usize, Option<usize>) {
        let left = self.size - self.rightOffset - self.leftPointer;
        return (left, Some(left));
    }
}

impl <'a,T> DoubleEndedIterator for MatrixComponentIterator<'a,T>
{
    fn next_back(&mut self) -> Option<Self::Item> {
        if self.size < self.rightOffset + self.leftPointer + 1
        {
            return None;
        }

        let (line, col) = self.getLineAndCol(self.size - self.rightOffset - 1)?;
        let ret = self.matrix.get(line, col);

        if ret.is_some()
        {
            self.rightOffset += 1;
        }

        return ret;
    }
}

impl <'a,T> ExactSizeIterator for MatrixComponentIterator<'a,T>
{}


struct MyMatrix
{
    val: Vec<Vec<i8>>
}

impl MyMatrix
{
    fn new() -> Self
    {
        return MyMatrix { val: Vec::new() };
    }

    fn addLine(&mut self, line: Vec<i8>)
    {
        self.val.push(line);
    }
}

impl SparseMatrix for MyMatrix
{
    type MatrixValueType = i8;

    fn getNumOfLines(&self) -> usize {
        return self.val.len();
    }

    fn getNumOfElInLine(&self, line: usize) -> usize {
        return self.val[line].len();
    }

    fn getLine(&self, line: usize) -> &Vec<i8> {
        return &self.val[line];
    }

    fn getLineMut(&mut self, line: usize) -> &mut Vec<i8> {
        return &mut self.val[line];
    }

    fn get (&self, line: usize, col: usize) -> Option<&i8> {
        return self.val.get(line)?.get(col);
    }

}

impl Matrix for MyMatrix {
    fn getNumOfCols(&self) -> usize {
        if self.val.is_empty()
        {
            return 0;
        }

        return self.getNumOfElInLine(0);
    }
}

#[derive(Clone)]
struct MyVisibilityLinesIterator<'a, T>
{
    visibilityLines: Vec<MyVisibilityLineIterator<'a, T>>
}

impl<'a, T> MyVisibilityLinesIterator<'a, T>
{
    fn new(matrix: &'a dyn Matrix<MatrixValueType = T>, elLine: usize, elCol: usize) -> Self
    {
        let mut v = Vec::new();
        v.push(MyVisibilityLineIterator::new(matrix, elLine, elCol, MyVisibilityLineItType::ToTheLeft));
        v.push(MyVisibilityLineIterator::new(matrix, elLine, elCol, MyVisibilityLineItType::ToTheRight));
        v.push(MyVisibilityLineIterator::new(matrix, elLine, elCol, MyVisibilityLineItType::ToTheTop));
        v.push(MyVisibilityLineIterator::new(matrix, elLine, elCol, MyVisibilityLineItType::ToTheBottom));

        return MyVisibilityLinesIterator
        { 
            visibilityLines: v
        };
    }

    fn iter(&self) -> Iter<MyVisibilityLineIterator<'a,T>>
    {
        return self.visibilityLines.iter();
    }
}

#[derive(Clone, Debug)]
enum MyVisibilityLineItType
{
    ToTheLeft,
    ToTheRight,
    ToTheTop,
    ToTheBottom,
}

#[derive(Clone)]
struct MyVisibilityLineIterator<'a, T>
{
    itType: MyVisibilityLineItType,
    matrix: &'a dyn Matrix<MatrixValueType = T>,
    elLine: usize,
    elCol: usize,
    forwardIt: usize,
    size: usize,
    backOffset: usize,
}

impl<'a, T> MyVisibilityLineIterator<'a, T>
{
    fn new(matrix: &'a dyn Matrix<MatrixValueType = T>, elLine: usize, elCol: usize, itType: MyVisibilityLineItType) -> Self
    {
        let size = match itType {
            MyVisibilityLineItType::ToTheLeft => elCol,
            MyVisibilityLineItType::ToTheRight => matrix.getNumOfCols() - elCol - 1,
            MyVisibilityLineItType::ToTheTop => elLine,
            MyVisibilityLineItType::ToTheBottom => matrix.getNumOfLines() - elLine - 1,
        };

        return MyVisibilityLineIterator
        {
            itType,
            matrix,
            elLine,
            elCol,
            forwardIt: 0,
            size,
            backOffset: 0,
        };
    }

    fn getLineAndCol(&self, newItPos: usize) -> Option<(usize, usize)>
    {
        return match self.itType {
            MyVisibilityLineItType::ToTheLeft => Some((self.elLine, self.elCol - newItPos - 1)),
            MyVisibilityLineItType::ToTheRight => Some((self.elLine, self.elCol + newItPos + 1)),
            MyVisibilityLineItType::ToTheTop => Some((self.elLine - newItPos - 1,self.elCol)),
            MyVisibilityLineItType::ToTheBottom => Some((self.elLine + newItPos + 1, self.elCol)),
        };
    }
}

impl<'a,T> Iterator for MyVisibilityLineIterator<'a,T>
{
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        if self.size < self.backOffset + self.forwardIt + 1
        {
            return None;
        }

        let (line, col) = self.getLineAndCol(self.forwardIt)?;
        let ret = self.matrix.get(line, col);

        if ret.is_some()
        {
            self.forwardIt += 1;
        }

        return ret;
    }

    fn size_hint(&self) -> (usize, Option<usize>) {
        let left = self.size - self.backOffset - self.forwardIt;
        return (left, Some(left));
    }
}

impl<'a,T> DoubleEndedIterator for MyVisibilityLineIterator<'a,T>
{
    fn next_back(&mut self) -> Option<Self::Item> {
        if self.size < self.backOffset + self.forwardIt + 1
        {
            return None;
        }

        let (line, col) = self.getLineAndCol(self.size - self.backOffset - 1)?;
        let ret = self.matrix.get(line, col);

        if ret.is_some()
        {
            self.backOffset += 1;
        }

        return ret;
    }
}

impl<'a,T> ExactSizeIterator for MyVisibilityLineIterator<'a,T>
{}

type ParsedType = MyMatrix; 
fn parseInput(filePath : &str) -> Option<ParsedType>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut retVal = MyMatrix::new();

    for line in lines.into_iter().map(|lr| lr.unwrap())
    {
        retVal.addLine(line.chars().map(|ch| ch.to_digit(10).unwrap() as i8).collect());
    }

    return Some(retVal);
}

fn countVisible1(matrix: &MyMatrix) -> u64
{
    let LINES = matrix.getNumOfLines();
    let COLS = matrix.getNumOfCols();

    let mut ret = ((LINES + COLS) as u64) * 2 - 4;

    let mut seen = HashSet::<(usize, usize)>::new();

    for line in 1..(LINES-1)
    {
        for col in 1..(COLS-1)
        {
            let currVal = matrix.wrap()[line][col];
            for visibilityLine in MyVisibilityLinesIterator::new(matrix, line, col).iter()
            {
                if seen.contains(&(line, col))
                {
                    break;
                }

                if visibilityLine.len() == 0 || visibilityLine.clone().all(|tree| tree < &currVal)
                {
                    // visible
                    ret += 1;
                    seen.insert((line, col));
                }
            }
        }
    }

    return ret;
}

fn countVisible2(matrix: &MyMatrix) -> u64
{
    let LINES = matrix.getNumOfLines();
    let COLS = matrix.getNumOfCols();

    let mut ret = 0;

    for line in 0..LINES
    {
        for col in 0..COLS
        {
            let currVal = matrix.wrap()[line][col];
            let mut treeScore = 1;
            for visibilityLine in MyVisibilityLinesIterator::new(matrix, line, col).iter()
            {
                if visibilityLine.len() == 0
                {
                    continue;
                }

                let mut visibleTrees = visibilityLine.clone().take_while(|val| **val < currVal).count() as u64;

                if visibleTrees < (visibilityLine.len() as u64)
                {
                    visibleTrees += 1;
                }

                treeScore *= visibleTrees;
            }
            if treeScore > ret
            {
                ret = treeScore;
            }
        }
    }

    return ret;
}

fn main() {
    let Some(mut mat) = parseInput("input.txt") else
    {
        println!("INPUT ERROR!");
        return;
    };

    let p1 = countVisible1(&mat);
    println!("P1:{}", p1);
    
    let p2 = countVisible2(&mat);
    println!("P2:{}", p2);
}