// Source: https://p5js.org/examples/interaction-kaleidoscope.html
// I used ChatGPT to help me with the glow-in-the-dark feature

let symmetry = 6;
let angle = 360 / symmetry;
let brushSizeSlider;
let colors = ['#FF0000', '#FF7F00', '#FFFF00', '#00FF00', '#0000FF', '#8B00FF']; // Rainbow colors
let currentColorIndex = 0;

function setup() {
  createCanvas(710, 710);
  angleMode(DEGREES);
  background(0);
  brushSizeSlider = createSlider(1, 32, 4, 0.1);
}
function saveFile() {
  save('design.jpg');
}
function clearScreen() {
  background(0);
}
function changeColor() {
  currentColorIndex = (currentColorIndex + 1) % colors.length;
}
function keyPressed() {
  if (key === 's' || key === 'S') {
    saveFile();
  } else if (keyCode === DELETE || keyCode === BACKSPACE) {
    clearScreen();
  } else if (key === ' ') {
    changeColor();
  }
}

function draw() {
  translate(width / 2, height / 2);
  if (mouseX > 0 && mouseX < width && mouseY > 0 && mouseY < height) {
    let mx = mouseX - width / 2;
    let my = mouseY - height / 2;
    let pmx = pmouseX - width / 2;
    let pmy = pmouseY - height / 2;
    if (mouseIsPressed) {
      for (let i = 0; i < symmetry; i++) {
        rotate(angle);
        let sw = brushSizeSlider.value();
        strokeWeight(sw);
        let glowColor = color(colors[currentColorIndex]);
        glowColor.setAlpha(50);
        stroke(glowColor);
        line(mx, my, pmx, pmy);
        push();
        scale(1, -1);
        line(mx, my, pmx, pmy);
        pop();
      }
    }
  }
}
