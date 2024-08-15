fn main() {
    let mut a : bool = false;
    let mut b : bool = true;
    let mut s : bool = false;
  
    s = a || b;
    println!("or{}", s);
  
    s = a && b;
    println!("and{}", s);
  
    s = 1 < 4;
    println!("<{}", s);
  
    s = 1 > 4;
    println!(">{}", s);
  
    s = 1 != 4;
    println!("!={}", s);
  
    s = 1 == 4;
    println!("=={}", s);
  }