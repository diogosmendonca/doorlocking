import React, { Component } from "react";

class Menu extends Component {

    render(){
        return(
            <>
                <a href="#menu" id="menuLink" className="menu-link">
                    <span></span>
                </a>

                <div id="menu">
                    <div class="pure-menu">
                        <a className="pure-menu-heading" href="#">DoorLocking.App</a>
                        <ul className="pure-menu-list">
                            <li className="pure-menu-item"><a href='/open_door' className="pure-menu-link">Open Door</a></li>
                            <li className="pure-menu-item"><a href='/register_user' className="pure-menu-link">New User</a></li>
                            <li className="pure-menu-item"><a href='/list_users' className="pure-menu-link">List/Change Users</a></li>
                            <li className="pure-menu-item"><a href='/logout_handler' className="pure-menu-link">Logout</a></li>
                        </ul>
                    </div>
                </div>
            </>
        );
    }
}

export default Menu;