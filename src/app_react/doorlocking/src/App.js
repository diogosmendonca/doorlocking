import React from 'react';
import './App.css';
import Menu from './components/MenuComponent';
import Header from './components/HeaderComponent';
import Footer from './components/FooterComponent';
import MenuContent from './components/MenuContentComponent';

function App() {
  return (
    <div id="layout">
      <Menu/>
      <div id="main">
        <Header/>
        <MenuContent/>
        <Footer/>
      </div>
    </div>
  );
}

export default App;
