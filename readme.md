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

<!-- Run test site -->
node app.js

<!-- Site address -->
http://localhost:3000