fn main() {
    let mut a: [i64; 5] = [1,3,5,7,9];
  
    for k in a {
      println!("{}", k);
    }
  
    for j in [10,13,2,8,9] {
      println!("{}", j);
    }
  
    for n in 1..4 {
      println!("{}", n);
    }
  
    for i in 1..=4 {
      println!("{}", i);
    }
  
  }