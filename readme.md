<!-- Run to access NVM from terminal -->

source $(brew --prefix nvm)/nvm.sh

<!-- Check Node Version to see if you are on Node 12.21.0 -->

node -v

<!-- List versions of Node installed -->

nvm ls

<!-- Install compatible version of Node -->

nvm install 12.21.0

<!-- Use Node 12.21.0, can run node -v again to double confirm -->

nvm use 12.21.0

<!-- Ensure arduino is transmitting value matched to image count -->

Machine 2: 64
Machine 3: 189
Machine 4: 30
Machine 6: 78

<!-- Run site using machine number -->

node 1.js
(for machine 2, use node 2.js)

<!-- Site address -->

http://localhost:3000
