import React, { Component } from "react";

class Header extends Component {
    render(){
        
        return(
            <div className="header">
                <h1>Menu</h1>
                {/* eslint-disable-next-line*/}
                <h4 id="msg"></h4>
            </div>
        );
    }
}

export default Header;