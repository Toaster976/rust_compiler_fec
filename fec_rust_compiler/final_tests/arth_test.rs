fn main() {
    let mut a: [i64; 5] = [1,3,5,7,9];
    let mut s : i64;
  
    s = a[4] + a[1];
    println!("+{}", s);
  
    s = a[4] - a[1];
    println!("-{}", s);
  
    s = a[4] * a[1];
    println!("*{}", s);
  
    s = a[4] / a[1];
    println!("/{}", s);
  
    s = a[4] % a[1];
    println!("%%{}", s);
  }
  